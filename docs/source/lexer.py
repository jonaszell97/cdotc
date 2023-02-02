from pygments.lexer import Lexer
from pygments.style import Style
from pygments import token
from codecs import encode, decode
import subprocess
import tempfile

class CDotLexer(Lexer):
    name = 'CDot'
    token_map = {
        # Various
        "ident": token.Generic,
        "op_ident": token.Operator,
        "dollar_ident": token.Keyword,
        "closure_arg": token.Keyword,
        "macro_name": token.Keyword,
        "percent_ident": token.Keyword,
        "dollar_dollar_ident": token.Keyword,
        "percent_percent_ident": token.Keyword,

        "line_comment": token.Comment,
        "block_comment": token.Comment,

        "eof": None,
        "sentinel": None,
        "expr_begin": token.Keyword,
        "stringify_begin": None,
        "preprocessor_value": None,
        "interpolation_begin": None,
        "interpolation_end": token.Keyword,
        "macro_expression": None,
        "macro_statement": None,
        "macro_declaration": None,

        # Literals
        "integerliteral": token.Number,
        "fpliteral": token.Number,
        "stringliteral": token.String,
        "charliteral": token.String,
        "kw_none": token.Literal,
        "kw_true": token.Literal,
        "kw_false": token.Literal,

        # Whitespace
        "space": token.Punctuation,

        # Punctuation
        "open_paren": token.Punctuation,
        "close_paren": token.Punctuation,
        "comma": token.Punctuation,
        "open_brace": token.Punctuation,
        "close_brace": token.Punctuation,
        "open_square": token.Punctuation,
        "close_square": token.Punctuation,
        "semicolon": token.Punctuation,
        "newline": token.Punctuation,
        "backslash": token.Punctuation,
        "period": token.Punctuation,
        "at": token.Punctuation,
        "pound": token.Punctuation,
        "single_quote": token.Punctuation,
        "double_quote": token.Punctuation,
        "backtick": token.Punctuation,
        "dollar": token.Punctuation,
        "underscore": token.Punctuation,

        # Operators
        "exclaim": token.Operator,
        "question": token.Operator,
        "colon": token.Operator,
        "equals": token.Operator,
        "plus_equals": token.Operator,
        "minus_equals": token.Operator,
        "times_equals": token.Operator,
        "div_equals": token.Operator,
        "mod_equals": token.Operator,
        "and_equals": token.Operator,
        "or_equals": token.Operator,
        "xor_equals": token.Operator,
        "exp_equals": token.Operator,
        "shl_equals": token.Operator,
        "ashr_equals": token.Operator,
        "lshr_equals": token.Operator,
        "logical_or": token.Operator,
        "logical_and": token.Operator,
        "op_and": token.Operator,
        "op_or": token.Operator,
        "caret": token.Operator,
        "tilde": token.Operator,
        "exclaim_equals": token.Operator,
        "double_equals": token.Operator,
        "triple_equals": token.Operator,
        "exclaim_double_equals": token.Operator,
        "smaller_equals": token.Operator,
        "greater_equals": token.Operator,
        "smaller": token.Operator,
        "greater": token.Operator,
        "shl": token.Operator,
        "lshr": token.Operator,
        "ashr": token.Operator,
        "plus": token.Operator,
        "minus": token.Operator,
        "times": token.Operator,
        "plus_plus": token.Operator,
        "minus_minus": token.Operator,
        "div": token.Operator,
        "percent": token.Operator,
        "times_times": token.Operator,
        "triple_period": token.Operator,
        "exclaim_is": token.Operator,
        "arrow_single": token.Operator,
        "arrow_double": token.Operator,
    }

    ident_map = {
        "String": token.Name,
        "Int": token.Name,
        "UInt": token.Name,
        "Int8": token.Name,
        "UInt8": token.Name,
        "Int16": token.Name,
        "UInt16": token.Name,
        "Int32": token.Name,
        "UInt32": token.Name,
        "Int64": token.Name,
        "UInt64": token.Name,
        "Int128": token.Name,
        "UInt128": token.Name,
        "Float": token.Name,
        "Double": token.Name,
        "Array": token.Name,
        "Dictionary": token.Name,
        "Option": token.Name,
        "print": token.Name,

        "self": token.Literal,
        "Self": token.Name,
        "super": token.Literal,
        "in": token.Keyword,
        "as": token.Keyword,
        "is": token.Keyword,
        "do": token.Keyword,
        "then": token.Keyword,
        "where": token.Keyword,
        "prefix": token.Keyword,
        "postfix": token.Keyword, 
        "infix": token.Keyword,
        "default": token.Keyword,
        "deinit": token.Keyword,
        "typename": token.Keyword,
        "sizeof": token.Keyword,
        "decltype": token.Keyword,
        "subscript": token.Keyword,
        "memberwise": token.Keyword,
        "get": token.Keyword,
        "set": token.Keyword,
        "read": token.Keyword,
        "write": token.Keyword,
        "nonmutating": token.Keyword,
        "virtual": token.Keyword,
        "override": token.Keyword,
        "with": token.Keyword,
        "throws": token.Keyword,
        "async": token.Keyword,
        "unsafe": token.Keyword,
        "precedenceGroup": token.Keyword,
        "higherThan": token.Keyword,
        "lowerThan": token.Keyword,
        "associativity": token.Keyword,
        "assignment": token.Keyword,
        "macro": token.Keyword,
        "owned": token.Keyword,
        "borrow": token.Keyword,
        "ref": token.Keyword,
        "mut": token.Keyword,
        "from": token.Keyword,
        "unittest": token.Keyword,
        "__traits": token.Name,
        "__nullptr": token.Literal,
        "__builtin_void": token.Name,
        "__mangled_func": token.Literal,
        "__ctfe": token.Literal,
    }

    def get_tokens_unprocessed(self, text):
        tmp_file = tempfile.NamedTemporaryFile(mode="w", delete=False)

        result = []
        try:
            tmp_file.write(text)
            tmp_file.flush()

            output = subprocess.check_output([
                'cdotc', tmp_file.name, '-emit-tokens',
            ]).decode('utf8')

            toks = output.split("\n")
            for tok in toks:
                firstComma = tok.find(",")
                offset = int(tok[1:firstComma])

                secondComma = tok.find(",", firstComma+1)
                type =  tok[firstComma+2:secondComma]

                spelling = decode(encode(tok[secondComma+2:-1], 'latin-1', 'backslashreplace'), 'unicode-escape')

                if type == "ident" and spelling in self.ident_map:
                    tok_type = self.ident_map[spelling]
                elif type in self.token_map:
                    tok_type = self.token_map[type]
                elif type.startswith("kw_"):
                    tok_type = token.Keyword
                else:
                    tok_type = None
                
                if tok_type is None:
                    continue

                result.append((offset, tok_type, spelling))
        except subprocess.CalledProcessError as err:
            print(err)
            tmp_file.close()

        return result


class CDotStyle(Style):
    default_style = ""
    styles = {
        token.Comment:                '#6a737d',
        token.Keyword:                '#d73a49',
        token.Generic:                '#24292e',
        token.Name:                   '#032f62',
        token.String:                 '#22863a',
        token.Literal:                '#005cc5',
        token.Number:                 '#005cc5',
    }
