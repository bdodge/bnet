
#include "battrparse.h"
#include "bipp.h"
#include "bippattr.h"
#include "bipperror.h"
#include "butil.h"

#define MAX_CSV_LINE	(256)

static size_t s_attr_index = 0;

static struct tag_name_to_xtype
{
	const char 	   *name;
	ipp_syntax_enc_t type;
}
s_syntax_to_type[] =
{
	{ "charset",		IPP_CHARSET 	},
	{ "naturalLanguage",IPP_LANGUAGE 	},
	{ "integer",		IPP_INTEGER		},
	{ "boolean",		IPP_BOOLEAN     },
	{ "enum",			IPP_ENUM        },
	{ "keyword",		IPP_KEYWORD     },

	{ "text",			IPP_TEXT        },
	{ "name",			IPP_NAME        },
	{ "octetString",	IPP_OCTETSTRING },
	{ "uri",			IPP_URI         },
	{ "uriScheme",		IPP_URISCHEME   },
	{ "mimeMediaType",	IPP_MIME        },

	{ "resolution",		IPP_RESOLUTION  },
	{ "rangeOfInteger",	IPP_RANGEOFINT  },
	{ "dateTime",		IPP_DATETIME    },
	{ "collection",		IPP_COLLECTION  },

	{ "1setOf",			IPP_ARRAY		}, // adds array bit
	{ "type1",			IPP_NOTYPE		}, // ignored
	{ "type2",			IPP_NOTYPE		}, // ignored
	{ "no-value",		IPP_NOTYPE		}, // ignored
	{ "unknown",		IPP_NOTYPE		}, // ignored
	{ "|",				IPP_NOTYPE		}, // advances type index
};

#define IPP_NUM_TYPE_ENCS (sizeof(s_syntax_to_type)/sizeof(struct tag_name_to_xtype))

int group_name_to_code(const char *name, ipp_attr_grouping_code_t *code)
{
	*code = 0;

	if (! name || ! code)
	{
		return -1;
	}
	if (! strcmp(name, "Document Description"))
	{
		*code = IPP_GROUPING_DOCUMENT_DESCRIPTION;
		return 0;
	}
	if (! strcmp(name, "Document Status"))
	{
		*code = IPP_GROUPING_DOCUMENT_STATUS;
		return 0;
	}
	if (! strcmp(name, "Document Template"))
	{
		*code = IPP_GROUPING_JOB_TEMPLATE;
		return 0;
	}
	if (! strcmp(name, "Event Notifications"))
	{
		*code = IPP_GROUPING_EVENT_NOTIFICATIONS;
		return 0;
	}
	if (! strcmp(name, "Job Description"))
	{
		*code = IPP_GROUPING_JOB_DESCRIPTION;
		return 0;
	}
	if (! strcmp(name, "Job Status"))
	{
		*code = IPP_GROUPING_JOB_STATUS;
		return 0;
	}
	if (! strcmp(name, "Job Template"))
	{
		*code = IPP_GROUPING_JOB_TEMPLATE;
		return 0;
	}
	if (! strcmp(name, "Operation"))
	{
		*code = IPP_GROUPING_OPERATION;
		return 0;
	}
	if (! strcmp(name, "Printer Description"))
	{
		*code = IPP_GROUPING_PRINTER_DESCRIPTION;
		return 0;
	}
	if (! strcmp(name, "Printer Status"))
	{
		*code = IPP_GROUPING_PRINTER_STATUS;
		return 0;
	}
	if (! strcmp(name, "Subscription Status"))
	{
		*code = IPP_GROUPING_SUBSCRIPTION_STATUS;
		return 0;
	}
	if (! strcmp(name, "Subscription Template"))
	{
		*code = IPP_GROUPING_SUBSCRIPTION_TEMPLATE;
		return 0;
	}
	butil_log(1, "Unknown Grouping: %s\n", name);
	return -1;
}

const char *group_code_to_name(ipp_attr_grouping_code_t code, bool plainname)
{
	if (plainname)
	{
		switch (code)
		{
		case IPP_GROUPING_DOCUMENT_DESCRIPTION: return "decdesc";
		case IPP_GROUPING_DOCUMENT_STATUS: return "docstat";
		case IPP_GROUPING_DOCUMENT_TEMPLATE: return "doctemp";
		case IPP_GROUPING_EVENT_NOTIFICATIONS: return "evtnot";
		case IPP_GROUPING_JOB_DESCRIPTION: return "jodesc";
		case IPP_GROUPING_JOB_STATUS: return "jobstat";
		case IPP_GROUPING_JOB_TEMPLATE: return "jobtemp";
		case IPP_GROUPING_OPERATION: return "operation";
		case IPP_GROUPING_PRINTER_DESCRIPTION: return "prtdesc";
		case IPP_GROUPING_PRINTER_STATUS: return "prtstat";
		case IPP_GROUPING_SUBSCRIPTION_STATUS: return "substat";
		case IPP_GROUPING_SUBSCRIPTION_TEMPLATE: return "subtemp";
		}
	}
	else
	{
		switch (code)
		{
		case IPP_GROUPING_DOCUMENT_DESCRIPTION: return "IPP_GROUPING_DOCUMENT_DESCRIPTION";
		case IPP_GROUPING_DOCUMENT_STATUS: return "IPP_GROUPING_DOCUMENT_STATUS";
		case IPP_GROUPING_DOCUMENT_TEMPLATE: return "IPP_GROUPING_DOCUMENT_TEMPLATE";
		case IPP_GROUPING_EVENT_NOTIFICATIONS: return "IPP_GROUPING_EVENT_NOTIFICATIONS";
		case IPP_GROUPING_JOB_DESCRIPTION: return "IPP_GROUPING_JOB_DESCRIPTION";
		case IPP_GROUPING_JOB_STATUS: return "IPP_GROUPING_JOB_STATUS";
		case IPP_GROUPING_JOB_TEMPLATE: return "IPP_GROUPING_JOB_TEMPLATE";
		case IPP_GROUPING_OPERATION: return "IPP_GROUPING_OPERATION";
		case IPP_GROUPING_PRINTER_DESCRIPTION: return "IPP_GROUPING_PRINTER_DESCRIPTION";
		case IPP_GROUPING_PRINTER_STATUS: return "IPP_GROUPING_PRINTER_STATUS";
		case IPP_GROUPING_SUBSCRIPTION_STATUS: return "IPP_GROUPING_SUBSCRIPTION_STATUS";
		case IPP_GROUPING_SUBSCRIPTION_TEMPLATE: return "IPP_GROUPING_SUBSCRIPTION_TEMPLATE";
		}
	}
	return "??????";
}

int syntax_name_to_code(char *name, ipp_syntax_enc_t type[IPP_MAX_ALT_TYPES])
{
	int result;
	size_t typedex;
	size_t cmpdex;
	ipp_syntax_enc_t typecode;
	ipp_syntax_enc_t tokentype;
	enum { tcpLeft, tcpRight } state;
	char *pn;
	char xn;

	for (typedex = 0; typedex < IPP_MAX_ALT_TYPES; typedex++)
	{
		type[typedex] = IPP_NOTYPE;
	}
	if (! name || ! type)
	{
		return -1;
	}
	typedex = 0;
	state = tcpLeft;
	typecode = IPP_NOTYPE;

	while (*name)
	{
		// skip any leading white space
		//
		for (pn = name; *pn && *pn == ' ' && *pn == '\t';)
		{
			pn++;
		}
		name = pn;

		// skip to first white space and terminate this type token
		//
		pn = name;
		while (
				*pn != '\0'
			&& 	*pn != ' '
			&& 	*pn != '\t'
			&& 	*pn != '('
			&&	*pn != ')'
		)
		{
			pn++;
		}
		xn = *pn;
		*pn = '\0';

		// convert the token to a type spec code
		//
		for (cmpdex = 0; cmpdex < IPP_NUM_TYPE_ENCS; cmpdex++)
		{
			if (! strcmp(name, s_syntax_to_type[cmpdex].name))
			{
				tokentype = s_syntax_to_type[cmpdex].type;
				break;
			}
		}
		*pn = xn;
		if (cmpdex >= IPP_NUM_TYPE_ENCS)
		{
			butil_log(1, "No such type: %s\n", name);
			return -1;
		}
		// skip to next non-white delim
		while (*pn == ' ' || *pn == '\t')
		{
			pn++;
		}
		if ((tokentype & ~IPP_ARRAY) != IPP_NOTYPE)
		{
			// this was an atomic typespec
			//
			// or in any accumulated typemods
			//
			typecode |= tokentype;

			// see if there is a range
			//
			if (*pn == '(')
			{
				while (*pn && *pn != ')')
				{
					pn++;
				}
				if (*pn == ')')
				{
					pn++;
				}
			}
			// add the type
			//
			type[typedex] = typecode;
			typedex++;
			if (typedex >= IPP_MAX_ALT_TYPES)
			{
				butil_log(1, "Too many different types\n");
				return -1;
			}
			typecode = IPP_NOTYPE;
		}
		else if (tokentype == IPP_ARRAY)
		{
			// typemod array, if already have a typemod
			// push, since this is an array of array
			//
			if (typecode != IPP_NOTYPE)
			{
				type[typedex] = typecode;
				typedex++;
				if (typedex >= IPP_MAX_ALT_TYPES)
				{
					butil_log(1, "Too many different types\n");
					return -1;
				}
			}
			typecode = tokentype;

			if (*pn == '(')
			{
				// open paren after array means, array of multiple
				// types, or array of array, so continue parsing
				//
				pn++;
			}
		}
		// skip any white
		while (*pn == ' ' || *pn == '\t' || *pn == ')')
		{
			pn++;
		}
		// move to next type spec
		//
		name = pn;
	}
	return 0;
}

const char *type_code_to_name(ipp_syntax_enc_t typecode, char *buffer, size_t nbuffer)
{
	size_t cmpdex;
	bool isarray;
	const char *codestr;

	if (typecode == IPP_NOTYPE)
	{
		snprintf(buffer, nbuffer, "IPP_NOTYPE");
		return buffer;
	}
	isarray = typecode & IPP_ARRAY;
	typecode &= ~IPP_ARRAY;

	if (typecode == IPP_NOTYPE)
	{
		snprintf(buffer, nbuffer, "IPP_ARRAY");
		return buffer;
	}
	switch (typecode)
	{
	case IPP_CHARSET:		codestr = "IPP_CHARSET"; break;
	case IPP_LANGUAGE:		codestr = "IPP_LANGUAGE"; break;
	case IPP_INTEGER:		codestr = "IPP_INTEGER"; break;
	case IPP_BOOLEAN:		codestr = "IPP_BOOLEAN"; break;
	case IPP_ENUM:			codestr = "IPP_ENUM"; break;
	case IPP_KEYWORD:		codestr = "IPP_KEYWORD"; break;

	case IPP_TEXT:			codestr = "IPP_TEXT"; break;
	case IPP_NAME:			codestr = "IPP_NAME"; break;
	case IPP_OCTETSTRING:	codestr = "IPP_OCTETSTRING"; break;
	case IPP_URI:			codestr = "IPP_URI"; break;
	case IPP_URISCHEME:		codestr = "IPP_URISCHEME"; break;
	case IPP_MIME:			codestr = "IPP_MIME"; break;

	case IPP_RESOLUTION:	codestr = "IPP_RESOLUTION"; break;
	case IPP_RANGEOFINT:	codestr = "IPP_RANGEOFINT"; break;
	case IPP_DATETIME:		codestr = "IPP_DATETIME"; break;
	case IPP_COLLECTION:	codestr = "IPP_COLLECTION"; break;

	default:				codestr = "????"; break;
	}
	snprintf(buffer, nbuffer, "%s%s", codestr,
			isarray ? " | IPP_ARRAY" : "");
	return buffer;
}

int attr_typestring(ipp_syntax_enc_t type[IPP_MAX_ALT_TYPES], char *buffer, size_t nbuffer)
{
	ipp_syntax_enc_t typecode;
	size_t typedex;
	size_t cmpdex;
	int room;
	int len;
	bool opened;

	room = nbuffer;

	buffer[0] = '\0';
	opened = false;

	for (typedex = 0; typedex < IPP_MAX_ALT_TYPES; typedex++)
	{
		typecode = type[typedex];

		if (typecode == IPP_NOTYPE)
		{
			break;
		}
		if (typecode & IPP_ARRAY)
		{
			len = snprintf(buffer, room, "1setOf ");
			buffer += len;
			room -= len;
		}
		typecode &= ~IPP_ARRAY;

		if (typecode == IPP_NOTYPE)
		{
			opened = true;
			len = snprintf(buffer, room, "( ");
			buffer += len;
			room -= len;
		}
		else
		{
			for (cmpdex = 0; cmpdex < IPP_NUM_TYPE_ENCS; cmpdex++)
			{
				if (s_syntax_to_type[cmpdex].type == typecode)
				{
					break;
				}
			}
			if (cmpdex >= IPP_NUM_TYPE_ENCS)
			{
				len = snprintf(buffer, room, "??? ");
				buffer += len;
				room -= len;
			}
			else
			{
				len = snprintf(buffer, room, "%s %s",
						s_syntax_to_type[cmpdex].name,
						(typedex < (IPP_MAX_ALT_TYPES - 1) ?
							((type[typedex + 1] != IPP_NOTYPE) ? "| " : "")
							:	"")
						);
				buffer += len;
				room -= len;
			}
		}
	}
	if (opened)
	{
		len = snprintf(buffer, room, "}");
		buffer += len;
		room -= len;
	}
	return 0;
}

typedef struct tag_art
{
	struct tag_art *left;
	struct tag_art *right;
	size_t index;
	ipp_attr_rec_t *attr;
}
art_t;

typedef struct tag_art_list
{
	struct tag_art_list *next;
	struct tag_art_list *parent;
	char   name[MAX_CSV_LINE];
	art_t *tree;
}
art_list_t;

static int format_colname(art_list_t *col, char *buffer, size_t nbuffer)
{
	const char *pn;
	size_t i;

	i = 0;

	while (col)
	{
		for (pn = col->name; *pn && i < nbuffer - 1; i++)
		{
			if (*pn == '-')
			{
				buffer[i] = '_';
			}
			else
			{
				buffer[i] = *pn;
			}
			pn++;
		}
		if (i >= (nbuffer - 3))
		{
			butil_log(0, "Overflow name format\n");
			return -1;
		}
		if (col->parent)
		{
			buffer[i++] = '_';
			buffer[i++] = '_';
		}
		col = col->parent;
	}
	buffer[i] = '\0';
	return 0;
}

static int format_colname_for_text(art_list_t *col, char *buffer, size_t nbuffer)
{
	const char *pn;
	size_t i;

	i = 0;

	while (col)
	{
		for (pn = col->name; *pn && i < nbuffer - 1; i++)
		{
			buffer[i] = *pn++;
		}
		if (i >= (nbuffer - 3))
		{
			butil_log(0, "Overflow name format\n");
			return -1;
		}
		if (col->parent)
		{
			buffer[i++] = '.';
		}
		col = col->parent;
	}
	buffer[i] = '\0';
	return 0;
}

int add_attr(art_t **tree, ipp_attr_rec_t *attr)
{
	art_t *ptree;
	art_t *art;
	int cmp;

	if (! *tree)
	{
		art = (art_t *)malloc(sizeof(art_t));
		if (! art)
		{
			return -1;
		}
		art->left = NULL;
		art->right = NULL;
		art->attr = attr;
		art->index = 0;
		*tree = art;
		return 0;
	}
	ptree = *tree;

	cmp = strcmp(attr->name, ptree->attr->name);
	if (cmp < 0)
	{
		return add_attr(&ptree->left, attr);
	}
	else if (cmp > 0)
	{
		return add_attr(&ptree->right, attr);
	}
	else
	{
		// ensure attrs type match?
		butil_log(5, "Duplicate attr %s\n", attr->name);
		return 0;
	}
}

static ipp_attr_rec_t *create_attr(
									const char *name,
									const char *member_name,
									ipp_syntax_enc_t types[IPP_MAX_ALT_TYPES],
									int32_t minval,
									int32_t maxval
									)
{
	ipp_attr_rec_t *attr;
	size_t i;

	attr = (ipp_attr_rec_t *)malloc(sizeof(ipp_attr_rec_t));
	if (! attr)
	{
		return NULL;
	}
	attr->name = malloc(strlen(name) + 1);
	if (! attr->name)
	{
		free(attr);
		return NULL;
	}
	strcpy((char*)attr->name, name);

	for (i = 0; i < IPP_MAX_ALT_TYPES; i++)
	{
		attr->syntax[i] = types[i];
	}
	attr->minval = minval;
	attr->maxval = maxval;
	return attr;
}

static void emit_attrs(FILE *outfile, art_t *tree)
{
	char tb1[32];
	char tb2[32];
	char tb3[32];
	char tb4[32];

	if (! tree)
	{
		return;
	}
	if (tree->left)
	{
		emit_attrs(outfile, tree->left);
	}
	tree->index = s_attr_index++;
	fprintf(outfile, "    {   // index %zu\n", tree->index);
	fprintf(outfile, "        \"%s\",   %s\n", tree->attr->name,
			tree->attr->name[0] == '*' ? "// indirect reference to" : "");
	fprintf(outfile, "        { %s,%s,%s,%s },\n",
			type_code_to_name(tree->attr->syntax[0], tb1, sizeof(tb1)),
			type_code_to_name(tree->attr->syntax[1], tb2, sizeof(tb2)),
			type_code_to_name(tree->attr->syntax[2], tb3, sizeof(tb3)),
			type_code_to_name(tree->attr->syntax[3], tb4, sizeof(tb4))
			);
	fprintf(outfile, "        %d,%d\n",
			tree->attr->minval, tree->attr->maxval);
	fprintf(outfile, "    },\n");

	if (tree->right)
	{
		emit_attrs(outfile, tree->right);
	}
}

static art_t *find_attr(art_t *tree, const char *name)
{
	int cmp;

	if (! tree)
	{
		return NULL;
	}
	cmp = strcmp(name, tree->attr->name);
	if (cmp < 0)
	{
		return find_attr(tree->left, name);
	}
	else if (cmp > 0)
	{
		return find_attr(tree->right, name);
	}
	return tree;
}

static void emit_attr_val(FILE *outfile, art_t *attr_tree, art_t *tree)
{
	art_t *art;
	size_t index;

	if (! tree || ! attr_tree)
	{
		return;
	}
	if (tree->left)
	{
		emit_attr_val(outfile, attr_tree, tree->left);
	}
	art = find_attr(attr_tree, tree->attr->name);
	if (art)
	{
		index = art->index;
	}
	else
	{
		butil_log(1, "Attr %s not in list\n", attr_tree->attr->name);
		return;
	}
	fprintf(outfile, "    {\n");
	fprintf(outfile, "        %zu, // %s\n", index, tree->attr->name);
	fprintf(outfile, "        %d,%d,\n", 0, 0);
	fprintf(outfile, "        NULL,NULL\n");
	fprintf(outfile, "    },\n");

	if (tree->right)
	{
		emit_attr_val(outfile, attr_tree, tree->right);
	}
}

void insert_collection(art_list_t **list, art_list_t *col)
{
	art_list_t *al;
	art_list_t *preval;
	int cmp;

	if (! *list)
	{
		*list = col;
		return;
	}
	preval = NULL;
	al = *list;

	while (al)
	{
		cmp = strcmp(col->name, al->name);
		//butil_log(8, "cmp %d =%s= to =%s=\n", cmp, col->name, al->name);
		if (cmp < 0)
		{
			col->next = al;
			if (! preval)
			{
				*list = col;
				return;
			}
			preval->next = col;
			return;
		}
		else if (cmp == 0)
		{
			butil_log(1, "Duplicate collection name: %s\n", col->name);
			return;
		}
		preval = al;
		al = al->next;
	}
	preval->next = col;
	col->next = NULL;
}

art_list_t *create_col(const char *name, art_list_t *parent)
{
	art_list_t *col;

	col = (art_list_t *)malloc(sizeof(art_list_t));
	if (! col)
	{
		butil_log(0, "Can't alloc collection\n");
		return NULL;
	}
	strncpy(col->name, name, sizeof(col->name) -1);
	col->name[sizeof(col->name) - 1] = '\0';
	col->parent = parent;
	col->tree = NULL;
	col->next = NULL;
	return col;
}

/*
* Parsing a CSV file like:
*
* groupname, name,member name, sub-member-name, syntax, reference
*/
int get_column(char *pl, char **epl, char *out, size_t nout)
{
	size_t i;

	if (! pl || ! out)
	{
		return -1;
	}
	i = 0;

	// copy column token to output
	while (
				*pl != '\0'
			&& 	*pl != ','
			&& 	*pl != '\r'
			&& 	*pl != '\n'
			&& 	i < (nout - 1)
	)
	{
		out[i++] = *pl++;
	}
	if (i >= (nout - 1))
	{
		butil_log(0, "token overflow: %s\n", pl);
		return -1;
	}
	// set epl to start of next column
	if (epl)
	{
		if (*pl == ',')
		{
			pl++;
			while (*pl == ' ' || *pl == '\t')
			{
				pl++;
			}
		}
		*epl = pl;
	}
	// trim trailing white space
	while (i > 0)
	{
		if (out[i - 1] == ' ' || out[i - 1] == '\t')
		{
			i--;
		}
		else
		{
			break;
		}
	}
	out[i] = '\0';
	butil_log(7, "column=%s=\n", out);
	return 0;
}

int iana_parse_attributes(
						const char *fname,
						const char *hdrname,
						FILE *infile,
						FILE *hdrfile,
						FILE *srcfile
						)
{
	char token[MAX_CSV_LINE];
	char attr_name[MAX_CSV_LINE];
	char syntax[MAX_CSV_LINE];
	char member_name[MAX_CSV_LINE];
	char sub_member_name[MAX_CSV_LINE];
	ipp_attr_grouping_code_t group_code;
	ipp_syntax_enc_t typespecs[IPP_MAX_ALT_TYPES];
	art_t *atree;
	art_t *gtrees[IPP_GROUPING_SUBSCRIPTION_TEMPLATE + 1];
	art_list_t *curcol;
	art_list_t *collections;
	art_t **ptree;
	ipp_attr_rec_t *attr;
	char line[MAX_CSV_LINE];
	size_t linenum;
	size_t typedex;
	char *pl;
	bool ismember;
	bool iscollection;
	int result;

	if (! infile || ! hdrfile || ! srcfile)
	{
		return -1;
	}
	linenum = 0;
	atree = NULL;
	collections = NULL;
	curcol = NULL;

	for (typedex = 0; typedex < IPP_GROUPING_SUBSCRIPTION_TEMPLATE + 1; typedex++)
	{
		gtrees[typedex] = NULL;
	}
	// read header
	pl = fgets(line, MAX_CSV_LINE, infile);
	linenum++;

	// uppercase format header file name
	strncpy(token, hdrname, sizeof(token) - 1);
	token[sizeof(token) - 1] = '\0';
	for (pl = token; *pl; pl++)
	{
		if (*pl >= 'a' && *pl <= 'z')
		{
			*pl = *pl - 'a' + 'A';
		}
		else if (*pl == '.')
		{
			*pl = '_';
		}
		else if (*pl == '-')
		{
			*pl = '_';
		}
	}
	// write header
	//
	fprintf(hdrfile, "/*\n * Generated File -- Consider not editing\n *\n*/\n");
	fprintf(hdrfile, "#ifndef %s\n#define %s 1\n\n", token, token);
	fprintf(hdrfile, "#include \"bippattr.h\"\n\n");

	fprintf(srcfile, "/*\n * Generated File -- Consider not editing\n */\n");
	fprintf(srcfile, "#include \"%s\"\n\n", hdrname);

	while (pl != NULL)
	{
		pl = fgets(line, MAX_CSV_LINE, infile);
		linenum++;
		if (! pl)
		{
			break;
		}
		if (pl[0] == '\r' || pl[0] == '\n' || pl[0] == '\0')
		{
			continue;
		}
		// group name
		result = get_column(pl, &pl, token, sizeof(token));
		if (result)
		{
			butil_log(1, "%s:%zu Missing group name", fname, linenum);
			return result;
		}
		result = group_name_to_code(token, &group_code);
		if (result)
		{
			butil_log(1, "%s:%zu Bad group name: %s", fname, linenum, token);
			return result;
		}
		// attr name
		result = get_column(pl, &pl, attr_name, sizeof(attr_name));
		if (result)
		{
			butil_log(1, "%s:%zu Missing attribute name", fname, linenum);
			return result;
		}
		// skip extensions/obsolete/deprecated
		//
		if (strstr(attr_name, "(deprecated)"))
		{
			butil_log(4, "Ingoring deprecated %s\n", attr_name);
			continue;
		}
		if (strstr(attr_name, "(obsolete)"))
		{
			butil_log(4, "Ingoring obsolete %s\n", attr_name);
			continue;
		}
		if (strstr(attr_name, "(extension)"))
		{
			butil_log(4, "Ingoring extension %s\n", attr_name);
			continue;
		}
		// member name
		result = get_column(pl, &pl, member_name, sizeof(member_name));
		if (result)
		{
			butil_log(1, "%s:%zu Missing member name", fname, linenum);
			return result;
		}
		// sub-member name
		result = get_column(pl, &pl, sub_member_name, sizeof(sub_member_name));
		if (result)
		{
			butil_log(1, "%s:%zu Missing sub-member name", fname, linenum);
			return result;
		}
		// type
		result = get_column(pl, &pl, syntax, sizeof(syntax));
		if (result)
		{
			butil_log(1, "%s:%zu Missing syntax", fname, linenum);
			return result;
		}
		if (! strstr(member_name, "<Member"))
		{
			result = syntax_name_to_code(syntax, typespecs);
			if (result)
			{
				butil_log(1, "%s:%zu bad typespec: %s", fname, linenum, syntax);
				return result;
			}
		}
		else
		{
			char *pref;

			// this is an indirect reference to another collection that
			// specifies the same members (i.e. this is a type reference)
			// so format the collection name a a reference string
			//
			member_name[0] = '*';

			// look forward for double quotes around coll name
			//
			pref = strstr(member_name, "\"\"");
			if (! pref)
			{
				butil_log(1, "%s:%zu No double quote in collection ref\n",
						fname, linenum);
				return -1;
			}
			// move the refname left
			memmove(member_name + 1, pref + 2, strlen(pref + 2));

			// look forward for ending double quotes
			//
			pref = strstr(member_name + 3, "\"\"");
			if (! pref)
			{
				butil_log(1, "%s:%zu No closing double quote in collection ref\n",
						fname, linenum);
				return -1;
			}
			// and terminate it
			//
			*pref = '\0';

			// and it has no type
			//
			for (typedex = 0; typedex < IPP_MAX_ALT_TYPES; typedex++)
			{
				typespecs[typedex] = IPP_NOTYPE;
			}
		}
		// reference
		result = get_column(pl, &pl, token, sizeof(token));
		if (result)
		{
			butil_log(1, "%s:%zu Missing reference", fname, linenum);
			return result;
		}
		attr_typestring(typespecs, token, sizeof(token));

		if (member_name[0])
		{
			ismember = true;

			if (! curcol)
			{
				// create a new collection for this first member
				//
				butil_log(5, " New Collection %s\n", attr_name);

				curcol = create_col(attr_name, NULL);
				if (! curcol)
				{
					return -1;
				}
				insert_collection(&collections, curcol);
			}
			if (sub_member_name[0])
			{
				if (! curcol->parent)
				{
					// first sub-collection member of a collection
					// so create a new collection parenting current
					//
					butil_log(5, "   New Sub-Collection %s\n", member_name);

					curcol = create_col(member_name, curcol);
					if (! curcol)
					{
						return -1;
					}
					insert_collection(&collections, curcol);
				}
				butil_log(5, "    SUBMEMB %s of %s .......... %s\n",
							sub_member_name, member_name, syntax);

				// make attr_name the member name for listing this in curcol
				//
				strcpy(attr_name, sub_member_name);
			}
			else
			{
				if (curcol->parent)
				{
					butil_log(5, "   End sub-collection %s\n", curcol->name);
					curcol = curcol->parent;
				}
				// if curcol has a parent, pop up, end of subcol
				//
				butil_log(5, "  MEMBER %s of %s .............. %s\n",
							member_name, attr_name, syntax);
				// make attr_name the member name for listing this in curcol
				//
				strcpy(attr_name, member_name);
			}
		}
		else
		{
			ismember = false;

			if (curcol)
			{
				butil_log(5, " - End collection %s\n", curcol->name);
				curcol = NULL;
			}
			butil_log(5, "ATTR %s %s .............. %s\n",
						attr_name, syntax, token);

			// add the non-member attribute to it's group tree
			//
			attr = create_attr(
							attr_name,
							member_name,
							typespecs,
							0,
							55
							);
			if (! attr)
			{
				butil_log(0, "Can't alloc attr\n");
				return -1;
			}
			ptree = &gtrees[group_code];
			result = add_attr(ptree, attr);
			if (result)
			{
				butil_log(0, "Can't add attr\n");
				return result;
			}
		}
		// create an attribute for the attr/memb/submemb
		//
		attr = create_attr(
						attr_name,
						member_name,
						typespecs,
						0,
						55
						);
		if (! attr)
		{
			butil_log(0, "Can't alloc attr\n");
			return -1;
		}
		if (ismember)
		{
			// this is a member of current collection
			// so add it to current collection tree
			//
			// add this collection member to current collection tree
			ptree = &curcol->tree;
		}
		else
		{
			// add attribute to attribute tree
			ptree = &atree;
		}
		result = add_attr(ptree, attr);
		if (result)
		{
			butil_log(0, "Can't add attr\n");
			return result;
		}
	}
	// go through collections and emit collection descriptors
	//
	for (curcol = collections; curcol; curcol = curcol->next)
	{
		result = format_colname(curcol, member_name, sizeof(member_name));
		if (result)
		{
			butil_log(0, "Format name %s for collection failed\n",
				curcol->name);
			return result;
		}
		result = format_colname_for_text(curcol, sub_member_name, sizeof(sub_member_name));
		if (result)
		{
			butil_log(0, "Format name %s for collection failed\n",
				curcol->name);
			return result;
		}
		fprintf(srcfile, "// Members of collection %s\n//\n",
				sub_member_name);
		fprintf(srcfile, "ipp_attr_rec_t s_ipp_col_%s[] = \n{\n",
				member_name);
		s_attr_index = 0;
		emit_attrs(srcfile, curcol->tree);
		fprintf(srcfile, "};\n\n");
	}
	// create a cross reference table of collection name
	// to collection record. This avoids direct linking of
	// the attributes to collections, and collections to
	// each other. the linking is possible but makes it much
	// harder to later update the generated file by hand,
	// and requires mutliple passes due to fwd references
	// the text-based run time linking is a but less efficient
	// but way easier to follow and understand
	//
	fprintf(hdrfile, "typedef struct tag_col_xref\n{\n");
	fprintf(hdrfile, "    char           *name;\n");
	fprintf(hdrfile, "    ipp_attr_rec_t *col_attr;\n");
	fprintf(hdrfile, "    size_t          num_members;\n");
	fprintf(hdrfile, "}\nipp_collection_xref_t;\n\n");

	fprintf(srcfile, "struct tag_col_xref ");
	fprintf(srcfile, "s_ipp_collection_xref[] =\n{\n");

	for (curcol = collections; curcol; curcol = curcol->next)
	{
		result = format_colname(curcol, member_name, sizeof(member_name));
		if (result)
		{
			butil_log(0, "Format name %s for collection failed\n",
				curcol->name);
			return result;
		}
		result = format_colname_for_text(curcol, sub_member_name, sizeof(sub_member_name));
		if (result)
		{
			butil_log(0, "Format name %s for collection failed\n",
				curcol->name);
			return result;
		}
		snprintf(token, sizeof(token),
				"(sizeof(s_ipp_col_%s)/sizeof(ipp_attr_rec_t))",
				member_name);
		fprintf(srcfile, "    { \"%s\", s_ipp_col_%s, %s },\n",
				sub_member_name, member_name, token);
	}
	fprintf(srcfile, "};\n\n");

	// go through attribute tree and emit alphabetized list
	//
	fprintf(srcfile, "ipp_attr_rec_t s_ipp_attributes[] =\n{\n");
	s_attr_index = 0;
	emit_attrs(srcfile, atree);
	fprintf(srcfile, "};\n\n");

	// for each group, output a group tree for holding values
	//
	for (typedex = 0; typedex < IPP_GROUPING_SUBSCRIPTION_TEMPLATE; typedex++)
	{
		fprintf(srcfile, "ipp_attr_t s_ipp_attr_group_%s[] =\n{\n",
				group_code_to_name(typedex, true));
		emit_attr_val(srcfile, atree, gtrees[typedex]);
		fprintf(srcfile, "};\n\n");
	}

	// finally, a cross refernce table from group type to group table
	//
	fprintf(hdrfile, "typedef struct tag_grp_xref\n{\n");
	fprintf(hdrfile, "    ipp_attr_grouping_code_t  group_pname;\n");
	fprintf(hdrfile, "    ipp_attr_t               *group_attrs;\n");
	fprintf(hdrfile, "    size_t                    num_attr;\n");
	fprintf(hdrfile, "}\nipp_group_xref_t;\n\n");

	fprintf(srcfile, "ipp_group_xref_t s_ipp_group_xref[] =\n{\n");

	for (typedex = 0; typedex < IPP_GROUPING_SUBSCRIPTION_TEMPLATE; typedex++)
	{
		fprintf(srcfile, "    { %s, s_ipp_attr_group_%s, "
				"(sizeof(s_ipp_attr_group_%s)/sizeof(ipp_attr_t)) },\n",
				group_code_to_name(typedex, false),
				group_code_to_name(typedex, true),
				group_code_to_name(typedex, true)
				);
	}
	fprintf(srcfile, "};\n\n");

	fprintf(hdrfile, "#endif\n");
	return 0;
}

