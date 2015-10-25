#include "include/ast.h"
#include "include/eval.h"

#include <cstdlib>
#include <cstring>

void ast_node::cp(ast_node *src)
{
    type = src->type;
    value = src->value;
    next_node = nullptr;
    child_node = nullptr;
    previous_node =nullptr;
    parent_node = nullptr;
    name_list = nullptr;
}

ast_node *ast_gen::make_node(ast_node *current_node, ast_node *content_node, ast_node::TYPE t)
{
    ast_node *new_node;
    if (current_node)
    {
        new_node = new ast_node(t, current_node, current_node->parent_node, current_node->name_list);
    }
    else
    {
        new_node = new ast_node(t, nullptr, content_node, content_node->name_list);
        content_node->child_node = new_node;
    }
    return new_node;
}

char *ast_gen::new_str_ref(int len)
{
    char *ts = new char[len];
    str_ref *new_ref = new str_ref();
    new_ref->str = ts;
    new_ref->next = str_list;
    str_list = new_ref;
    return ts;
}

ast_node *ast_gen::make_name(ast_node *current_node, ast_node *parent_node)
{
    ast_node *name_node = make_node(current_node, parent_node, ast_node::TYPE::NAME);
    ast_node *value_node = nullptr;
    while (parsing->type == token_list_elem::RAW_TEXT || parsing->type == token_list_elem::VAR_DEFINE)
    {
        if (parsing->type == token_list_elem::RAW_TEXT)
        {
            value_node = make_node(value_node, name_node, ast_node::TYPE::RAW_TEXT);
            value_node->value = parsing->content;
        }
        else
        {
            value_node = make_node(value_node, name_node, ast_node::TYPE::VAR_CALL);
            value_node->var_call = make_var_call();

        }
        parsing = parsing->next;
    }
    return name_node;
}


_operation *ast_gen::make_operation()
{
    _operation *result=new _operation(), *current=result;

#define NUM 0
#define OP 1
#define OTHER 2

    int c;
    switch (parsing->type)
    {
    case token_list_elem::INT:
    {
        current->type = _operation::INT;
        current->i_val = atoi(parsing->content);
        c = NUM;
        break;
    }
    case token_list_elem::FLOAT:
    {
        current->type = _operation::FLOAT;
        current->f_val = atof(parsing->content);
        c=NUM;
        break;
    }
    case token_list_elem::COLOR:
    {
        current->type = _operation::COLOR;
        current->i_val = strtoul(parsing->content+1, nullptr, 16);
        c=NUM;
        break;

    }
    case token_list_elem::VAR_DEFINE:
    {
        current->type = _operation::VAR_CALL;
        current->var = make_var_call();
        c=NUM;
        break;
    }
    case token_list_elem::PX:
    {
        current->type = _operation::PX;
        current->f_val = atof(parsing->content);
        c=NUM;
        break;
    }
    case token_list_elem::OPERATOR:
    {
        current->type = _operation::OPERATOR;
        current->op = *(parsing->content);
        c=OP;
        break;
    }
    case token_list_elem::STRING:
    {
        current->type = _operation::STRING;
        current->s_string = parsing->content;
        c=OTHER;
    }
    default:
        delete result;
        return nullptr;
    }
    current->next = nullptr;
    parsing = parsing->next;
    while (c!=OTHER)
    {
        _operation *new_current=new _operation();
        switch (parsing->type)
        {
        case token_list_elem::INT:
        {
            if (c == OP)
            {
                new_current->type = _operation::INT;
                new_current->i_val = atoi(parsing->content);
                c = NUM;
                break;
            }
            else
            {
                delete new_current;
                return result;
            }
        }
        case token_list_elem::FLOAT:
        {
            if (c == OP)
            {
                new_current->type = _operation::FLOAT;
                new_current->f_val = atof(parsing->content);
                c = NUM;
                break;
            }
            else
            {
                delete new_current;
                return result;
            }
        }
        case token_list_elem::COLOR:
        {
            if (c == OP)
            {
                new_current->type = _operation::COLOR;
                new_current->i_val = strtoul(parsing->content+1, nullptr, 16);
                c = NUM;
                break;
            }
            else
            {
                delete new_current;
                return result;
            }
        }
        case token_list_elem::VAR_DEFINE:
        {

            if (c == OP)
            {
                new_current->type = _operation::VAR_CALL;
                new_current->var = make_var_call();
                c = NUM;
                break;
            }
            else
            {
                delete new_current;
                return result;
            }
            break;
        }
        case token_list_elem::OPERATOR:
        {
            if (c == NUM)
            {
                new_current->type = _operation::OPERATOR;
                new_current->op = *(parsing->content);
                c = OP;
                break;
            }
            else
            {
                delete new_current;
                return result;
            }
        }
        default:
            delete new_current;
            return result;
        }
        current->next = new_current;
        current = new_current;
        current->next = nullptr;
        parsing = parsing->next;
    }
    return result;
}

_var_call *ast_gen::make_var_call()
{
    _var_call *var_call = new _var_call();
    var_call->undef_times = 0;
    while (parsing->type == token_list_elem::VAR_DEFINE)
    {
        ++var_call->undef_times;
        parsing = parsing->next;
        if (parsing->type == token_list_elem::BLOCK_BEGIN)
            parsing = parsing->next;
    }
    while (parsing->type == token_list_elem::BLOCK_END)
        parsing = parsing->next;
    var_call->name = parsing->content;
    parsing = parsing->next;
    return var_call;
}

void ast_gen::end_block(ast_node *content_node)
{
    _var *new_name = new _var(_var::FUNC);
    new_name->func = content_node->parent_node;
    new_name->next = content_node->name_list->next;
    content_node->name_list->next = new_name;
    if (content_node->parent_node->type == ast_node::TYPE::FUNC_DEFINE)
    {
        new_name->func->next_node = func_node;
        func_node = new_name->func;
    }
    parsing = parsing->next;
    func_call_extend(content_node);
    if (new_name->func->type == ast_node::TYPE::FUNC_DEFINE)
    {
        int args = 0;
        ast_node *arg;
        for (arg = new_name->func; arg; arg = arg->next_node)
        {
            if (arg->type == ast_node::TYPE::FUNC_ARGUMENT)
                ++args;
        }
        char **mask = new char*[args];
        args = 0;
        for (arg = new_name->func; arg; arg = arg->next_node)
        {
            if (arg->type == ast_node::TYPE::FUNC_ARGUMENT)
            {
                mask[args] = arg->child_node->value;
                ++args;
            }
        }
        property_extend(content_node, args, mask);
        //var_extend(content_node,args, mask);
        delete mask;
    }
    else
    {
        property_extend(content_node, 0, nullptr);
        //var_extend(content_node, 0, nullptr);
    }
}

int ast_gen::sub_block_parser(ast_node *content_node)
{
    ast_node *current_node = nullptr;
    while (parsing->type != token_list_elem::BLOCK_END)
    {
        switch (parsing->type)
        {
        case token_list_elem::RAW_TEXT:
        case token_list_elem::COMMENT:
        {
            ast_node *new_node = make_node(current_node, content_node, ast_node::TYPE::RAW_TEXT);
            new_node->value = parsing->content;
            current_node = new_node;
            parsing = parsing->next;
            break;
        }
        case token_list_elem::SELECTOR_NAME:
        {
            ast_node *new_node = make_node(current_node, content_node, ast_node::TYPE::SELECTOR);
            parsing = parsing->next;
            ast_node *name_node = nullptr;
            while (parsing->type != token_list_elem::BLOCK_BEGIN)
            {
                if (parsing->type == token_list_elem::SELECTOR_NAME)
                {
                    name_node = make_name(name_node, new_node);
                }
                parsing = parsing->next;
            }
            _var *new_name_list = new _var(_var::NEW_FLAG);
            new_name_list->next = current_node->name_list;
            ast_node *new_content = new ast_node(ast_node::TYPE::CONTENT, name_node, new_node, new_name_list);
            name_node->next_node = new_content;
            int ret = sub_block_parser(new_content);
            if (ret)
                return ret;
            parsing = parsing->next;
            current_node = new_node;
            break;
        }
        case token_list_elem::BLOCK_BEGIN:
            return -2;
            break;
        case token_list_elem::SEPARATOR:
            parsing = parsing->next;
            continue;
        case token_list_elem::VAR_DEFINE:
        {
            if (parsing->next->type == token_list_elem::RAW_TEXT)
            {
                parsing = parsing->next;
                _var *new_name = new _var(_var::UNDECIDED);
                new_name->name = parsing->content;
                new_name->next = current_node->name_list->next;
                current_node->name_list->next = new_name;
                parsing = parsing->next;
                if (parsing->type != token_list_elem::ASSIGN)
                    return -4;
                parsing = parsing->next;
                new_name->op_list = make_operation();
                new_name->type = _var::UNDECIDED;
                _var *t1, *t2;
                for (t1=new_name, t2=new_name->next; !(t2->type == _var::NEW_FLAG); t1=t1->next, t2=t2->next)
                {
                    t1->next = t2->next;
                    delete t2;
                }
                break;
            }
            else if (parsing->type != token_list_elem::BLOCK_BEGIN)
            {
                return -5;
            }

        }
        case token_list_elem::PROPERTY_NAME:
        {
            ast_node *new_node = make_node(current_node, content_node, ast_node::TYPE::PROPERTY);
            parsing = parsing->next;
            ast_node *name_node = make_name(nullptr, new_node);
            if (parsing->type != token_list_elem::ASSIGN)
                return -6;
            ast_node *value_node = name_node;
            while (parsing->type != token_list_elem::SEPARATOR)
            {
                value_node= new ast_node(ast_node::TYPE::OPERATION, value_node, new_node, current_node->name_list);
                value_node->op = make_operation();
            }
            current_node = new_node;
            break;
        }
        case token_list_elem::FUNC_NAME:
        {
            token_list_elem *t = parsing->next;
            while ((t->type != token_list_elem::BLOCK_BEGIN) && (t->type != token_list_elem::SEPARATOR))
                t = t->next;
            if (t->type == token_list_elem::BLOCK_BEGIN)
            {
                ast_node *new_func = new ast_node(ast_node::TYPE::FUNC_DEFINE, nullptr, nullptr, current_node->name_list);
                parsing = parsing->next;
                ast_node *name_node = make_name(nullptr, new_func);

                if (parsing->type != token_list_elem::FUNC_ARGUMENT_BEGIN)
                    return -7;
                parsing = parsing->next;
                ast_node *arg = name_node;
                while (parsing->type == token_list_elem::VAR_DEFINE)
                {
                    arg = new ast_node(ast_node::TYPE::FUNC_ARGUMENT, arg, new_func, arg->name_list);

                    parsing = parsing->next;
                    ast_node *arg_name = new ast_node(ast_node::TYPE::RAW_TEXT, nullptr, arg, arg->name_list);
                    arg_name->value = parsing->content;

                    parsing = parsing->next;
                    if (parsing->type == token_list_elem::ASSIGN)
                    {
                        ast_node *v_default = new ast_node(ast_node::TYPE::OPERATION, arg_name, arg, arg->name_list);
                        v_default->op = make_operation();
                    }
                    if (parsing->type == token_list_elem::COMMA)
                        parsing = parsing->next;
                }
                if (parsing->type != token_list_elem::FUNC_ARGUMENT_END)
                    return -8;
                parsing = parsing->next;
                if (parsing->type != token_list_elem::BLOCK_BEGIN)
                    return -9;
                parsing = parsing->next;
                _var *new_name_list = new _var(_var::NEW_FLAG);
                ast_node *new_content = new ast_node(ast_node::TYPE::CONTENT, arg, new_func, new_name_list);
                int flag = sub_block_parser(new_content);
                if (flag)
                    return flag;
                break;

            }
            else
            {
                ast_node *func_call = make_node(current_node, content_node, ast_node::TYPE::FUNC_CALL);
                parsing = parsing->next;
                ast_node *call_name = make_node(nullptr, func_call, ast_node::TYPE::NAME);
                ast_node *name_value = make_node(nullptr, call_name, ast_node::TYPE::RAW_TEXT);
                func_call->child_node = call_name;
                name_value->value = parsing->content;
                if (parsing->type == token_list_elem::FUNC_ARGUMENT_BEGIN)
                {
                    parsing = parsing->next;
                    ast_node *arg_node = call_name;
                    while (parsing->type != token_list_elem::FUNC_ARGUMENT_END)
                    {
                        arg_node = new ast_node(ast_node::TYPE::FUNC_ARGUMENT, arg_node, func_call, current_node->name_list);
                        if (parsing->type == token_list_elem::RAW_TEXT)
                        {
                            ast_node *v_text = new ast_node(ast_node::TYPE::RAW_TEXT, nullptr, arg_node, current_node->name_list);
                            v_text->value = parsing->content;
                            arg_node->child_node = v_text;
                        }
                        else
                        {
                            ast_node *op = new ast_node(ast_node::TYPE::OPERATION, nullptr, arg_node, current_node->name_list);
                            op->op = make_operation();
                            arg_node->child_node = op;
                        }
                        if (parsing->type == token_list_elem::COMMA)
                            parsing = parsing->next;
                    }
                    break;
                }
                parsing = parsing->next;
                current_node = func_call;
            }
        }
        default:
            parsing = parsing->next;
        }
    }
    end_block(content_node);
    return 0;
}

int ast_gen::first_step()
{
    return sub_block_parser(root_node);
}

void ast_gen::op_extend(ast_node *node, int mask_num, char **mask)
{
    _ret_with_type t = cal_op_string(node->op, node->name_list, mask_num, mask);
    char *result = t.s_result;
    if (result)
    {
        node->type = ast_node::TYPE::RAW_TEXT;
        if (t.t == _var::STRING)
        {
            char *ts = new_str_ref(strlen(result) + 3);
            char *tts = ts;
            *tts++ = '"';
            while (*result)
                *tts++ = *result++;
            *tts = '"';
            *(tts + 1) = 0;
            node->value = ts;
        }
        else if (t.t == _var::PX)
        {
            char *ts = new_str_ref(strlen(result) + 4), *dst = ts, *src = result;
            while (*src)
                *dst++ = *src++;
            *dst = ' ';
            *(dst+1) = 'p';
            *(dst+2) = 'x';
            *(dst+3) = 0;
            node->value = ts;
        }
        else
            node->value = result;

        node->child_node = nullptr;
    }
}



/*********TODO*************/
_ret_with_type ast_gen::cal_op_string(_operation *op, _var *var_list, int mask_num, char **mask)
{
    _operation *t;
    _operation::TYPE type = _operation::UNDECIDED;
    for (t = op; t; t = t->next)
    {
        if (t->type == _operation::VAR_CALL)
        {
            _ret_with_type var_temp = var_call_in(t->var, var_list, mask_num, mask);
            if (var_temp.s_result)
            {
                switch (var_temp.t)
                {
                case _var::INT:
                    t->type = _operation::INT;
                    t->i_val = var_temp.i_result;
                    break;
                case _var::FLOAT:
                    t->type = _operation::FLOAT;
                    t->f_val = var_temp.f_result;
                    break;
                case _var::COLOR:
                    t->type = _operation::COLOR;
                    t->i_val = var_temp.i_result;
                    break;
                case _var::PX:
                    t->type = _operation::PX;
                    t->f_val = var_temp.f_result;
                    break;
                case _var::STRING:
                    t->type = _operation::STRING;
                    t->s_string = var_temp.s_result;
                    break;
                case _var::RAW_TEXT:
                    t->type = _operation::RAW_TEXT;
                    t->text = var_temp.s_result;
                    break;
                default:
                    break;
                }
            }
            else
            {
                type = _operation::VAR_CALL;
            }
        }

        if (type == _operation::UNDECIDED)
            type = t->type;
        if (type == _operation::OPERATOR)
            type = _operation::UNDECIDED;
    }
    _ret_with_type ret;
    if (type == _operation::UNDECIDED || type == _operation::VAR_CALL)
    {
        ret.s_result = nullptr;
        ret.t = _var::UNDECIDED;
        return ret;
    }
    char *expr = new char[1024], *now = expr;
    switch (type)
    {
    case _operation::INT:
    {
        for (t = op; t; t = t->next)
        {
            if (t->type == _operation::OPERATOR)
                *now++ = t->op;
            else
                now += sprintf(now, "%d", t->i_val);
        }
        ret.t = _var::INT;
        ret.i_result = Compute_int(expr);
    }
    case _operation::FLOAT:
    {
        for (t = op; t; t = t->next)
        {
            if (t->type == _operation::OPERATOR)
                *now++ = t->op;
            else
                now += sprintf(now, "%f", t->f_val);
        }
        ret.t = _var::FLOAT;
        ret.f_result = Compute_float(expr);
    }
    case _operation::COLOR:
    {
        for (t = op; t; t = t->next)
        {
            if (t->type == _operation::OPERATOR)
                *now++ = t->op;
            else
                now += sprintf(now, "%i", t->i_val);
        }
        ret.t = _var::COLOR;
        ret.i_result = Compute_int(expr); //compute_color
    }
    case _operation::PX:
    {
        for (t = op; t; t = t->next)
        {
            if (t->type == _operation::OPERATOR)
                *now++ = t->op;
            else
                now += sprintf(now, "%f", t->f_val);
        }
        ret.t = _var::PX;
        ret.f_result = Compute_float(expr);
    }
    case _operation::STRING:
    {
        ret.t = _var::STRING;
        ret.s_result = op->s_string;
    }
    case _operation::RAW_TEXT:
    {
        ret.t = _var::RAW_TEXT;
        ret.s_result = op->text;
    }
    default:
        break;
    }
    delete expr;
    return ret;
}

_ret_with_type ast_gen::cal_var(char *var_name, _var *var_list, int mask_num, char **mask)
{
    int i;
    _ret_with_type ret;
    for (i=0; i < mask_num; ++i)
    {
        if (!strcmp(var_name, mask[i]))
        {
            ret.s_result = nullptr;
            ret.t = _var::UNDECIDED;
            return ret;
        }
    }
    _var *t;
    for (t = var_list; t; t = t->next)
        if ((t->type != _var::TYPE::NEW_FLAG) && (t->type != _var::TYPE::FUNC) && (!strcmp(var_name, t->name)))
            break;

    if (t->op_list)
    {
        ret = cal_op_string(t->op_list, var_list, mask_num, mask);
        if (ret.t != _var::UNDECIDED)
        {
            t->type = ret.t;
            t->op_list = nullptr;
            t->v_text = ret.s_result;
        }

    }
    else
    {
        ret.s_result = t->v_text;
        ret.t = t->type;
        switch (t->type)
        {
        case _var::FLOAT:
        case _var::PX:
        {
            ret.f_result = atof(ret.s_result);
            break;
        }
        case _var::COLOR:
        case _var::INT:
        {
            ret.i_result = atoi(ret.s_result);
            break;
        }
        default:
            ret.t = _var::UNDECIDED;
            ret.s_result = nullptr;
        }
    }
    return ret;
}

_ret_with_type ast_gen::var_call_in(_var_call *call, _var *var_list, int mask_num, char **mask)
{
    _ret_with_type t;
    char *var_name = call->name;
    int times = call->undef_times;
    while (times)
    {
        t = cal_var(var_name, var_list, mask_num, mask);
        var_name = t.s_result;
        if (!var_name)
            break;
        --times;
    }
    return t;
}

int ast_gen::var_extend(ast_node *node, int mask_num, char **mask)
{
    if (node->type == ast_node::VAR_CALL)
    {
        _ret_with_type t = var_call_in(node->var_call, node->name_list, mask_num, mask);
        char *result = t.s_result;
        if (result)
        {
            node->type = ast_node::TYPE::RAW_TEXT;
            if (t.t == _var::STRING)
            {
                char *ts = new_str_ref(strlen(result) + 3);
                char *tts = ts;
                *tts++ = '"';
                while (*result)
                    *tts++ = *result++;
                *tts = '"';
                *(tts + 1) = 0;
                node->value = ts;
            }
            else if (t.t == _var::PX)
            {
                char *ts = new_str_ref(strlen(result) + 4), *dst = ts, *src = result;
                while (*src)
                    *dst++ = *src++;
                *dst = ' ';
                *(dst+1) = 'p';
                *(dst+2) = 'x';
                *(dst+3) = 0;
                node->value = ts;
            }
            else
                node->value = result;
            find_merge(node);
            node->child_node = nullptr;
        }
    }
    int flag;
    if (node->child_node)
    {
        flag = var_extend(node->child_node, mask_num, mask);
        if (!flag)
            return flag;
    }
    if (node->next_node)
    {
        flag = var_extend(node->next_node, mask_num, mask);
        if (!flag)
            return flag;
    }
    return 0;
}

void ast_gen::find_merge(ast_node *node)
{
    ast_node *start = node, *end = node;
    int len = strlen(node->value) + 1;
    while (start->previous_node && (start->previous_node->type == ast_node::TYPE::RAW_TEXT))
    {
        start = start->previous_node;
        len += strlen(start->value);
    }
    while (end->next_node && (end->next_node->type == ast_node::TYPE::RAW_TEXT))
    {
        end = end->previous_node;
        len += strlen(end->value);
    }
    if (start != end)
    {
        char *new_char = new_str_ref(len), *t_char = new_char;

        ast_node *t_node = start;
        while (t_node != end->next_node)
        {
            char *s = t_node->value;
            while (*s)
                *t_char++ = *s++;
            t_node = t_node->next_node;
        }
        *t_char = 0;
        start->value = new_char;
        t_node = start->next_node;
        start->next_node = nullptr;
        while (t_node != end->next_node)
        {
            ast_node *t = t_node->next_node;
            delete t_node;
            t_node = t;
        }
    }
}

int ast_gen::check_arg(ast_node *func, ast_node *call)
{
    ast_node *t;
    int l1=0, l2=0;
    for (t = func->child_node; t; t = t->next_node)
        if (t->type == ast_node::TYPE::FUNC_ARGUMENT)
            ++l1;

    for (t = call->child_node; t; t = t->next_node)
        if (t->type == ast_node::TYPE::FUNC_ARGUMENT)
            ++l2;
    return (l1 == l2);

}

void ast_gen::add_new_name(_var *dst, _var *src)
{
    _var *t;
    _var *add_point;
    for (add_point = dst; add_point->next->type != _var::NEW_FLAG; add_point = add_point->next)
        ;
    for (t = src->next; t->type != _var::NEW_FLAG; t = t->next)
    {
        _var *t2;
        int valid = 1;
        for (t2 = dst; t2; t2=t2->next)
        {
            if (t2->type != _var::NEW_FLAG)
                if (!strcmp(t2->name, t->name))
                {
                    valid = 0;
                    break;
                }
        }
        if (valid)
        {
            _var *new_var = new _var(t->type);
            new_var->name = t->name;
            new_var->op_list = t->op_list;
            new_var->v_string = t->v_string;
            new_var->next = add_point->next;
            add_point->next = new_var;
        }
    }
}

void ast_gen::copy_child(ast_node *dst, ast_node *src, _var *var_list)
{
    if (!src)
    {
        dst->child_node = nullptr;
        return;
    }
    ast_node *s_copying = src->child_node, *d_copying;
    dst->child_node = new ast_node(s_copying->type, nullptr, dst, var_list);
    dst->child_node->value = s_copying->value;
    copy_child(dst->child_node, s_copying, var_list);
    d_copying = dst->child_node;
    s_copying = s_copying->next_node;
    while (s_copying)
    {
        d_copying->next_node = new ast_node(s_copying->type, d_copying, src, var_list);
        d_copying->next_node->value = s_copying->value;
        copy_child(d_copying->next_node, s_copying, var_list);
        d_copying = d_copying->next_node;
        s_copying = s_copying->next_node;
    }
}

void ast_gen::extend_with_var_list(ast_node *node, _var *var_list, ast_node *func)
{
    ast_node *next_node = node->next_node;
    ast_node *parent_node = node->parent_node;
    ast_node *content_node;
    for (content_node=func->child_node; content_node->type!= ast_node::TYPE::CONTENT; content_node = content_node->next_node)
        ;
    add_new_name(var_list, content_node->name_list);
    ast_node *src = content_node->child_node;
    ast_node *dst = node;
    if (src)
    {
        dst->cp(src);
        dst->previous_node = nullptr;
        dst->parent_node = parent_node;
        dst->name_list = var_list;
        copy_child(dst, src, var_list);
    }
    src = src->next_node;
    while (src)
    {
        dst->next_node = new ast_node(src->type, dst, dst->parent_node, var_list);
        dst->next_node->value = src->value;
        copy_child(dst->next_node, src, var_list);
        dst = dst->next_node;
        src = src->next_node;
    }
    dst->next_node = next_node;
}

int ast_gen::func_call_extend(ast_node *node)
{
    if (node->type == ast_node::FUNC_CALL)
    {
        char *func_name = node->child_node->child_node->value;
        _var *t;
        int found = 0;
        for (t = node->name_list; t; t = t->next)
        {
            if (t->type == _var::FUNC)
            {
                ast_node *name_node;
                for (name_node = t->func->child_node; name_node; name_node = name_node->next_node)
                {
                    if (name_node->type == ast_node::TYPE::NAME)
                    {
                        if ((name_node->child_node->type == ast_node::RAW_TEXT) && !(name_node->child_node->next_node))
                            if (!strcmp(func_name, name_node->child_node->value))
                            {
                                _var *tmp_var_list;
                                if (t->func->type == ast_node::TYPE::FUNC_DEFINE)
                                {
                                    _var *arg_var = new _var(_var::NEW_FLAG), *tmp=new _var(_var::UNDECIDED);
                                    ast_node *arg_count;
                                    arg_var->next = tmp;
                                    for (arg_count = node->child_node->next_node; arg_count; arg_count = arg_count->next_node)
                                        if (arg_count->type == ast_node::TYPE::FUNC_ARGUMENT)
                                        {
                                            _var *new_var = new _var(_var::UNDECIDED);
                                            new_var->name = arg_count->child_node->value;
                                            if (arg_count->child_node->next_node)
                                            {
                                                switch (arg_count->child_node->next_node->type)
                                                {
                                                case ast_node::TYPE::RAW_TEXT:
                                                    new_var->type = _var::RAW_TEXT;
                                                    new_var->v_text = arg_count->child_node->next_node->value;
                                                    break;
                                                case ast_node::TYPE::STRING:
                                                    new_var->type = _var::STRING;
                                                    new_var->v_string = arg_count->child_node->next_node->value;
                                                    break;
                                                default:
                                                    new_var->type = _var::UNDECIDED;
                                                    new_var->op_list = arg_count->child_node->next_node->op;
                                                }
                                            }
                                            tmp->next = new_var;
                                            tmp = new_var;
                                        }
                                    if (check_arg(t->func, node))
                                    {
                                        tmp = arg_var;
                                        for (arg_count = node->child_node->next_node; arg_count; arg_count = arg_count->next_node)
                                        {
                                            if (arg_count->type == ast_node::TYPE::FUNC_ARGUMENT)
                                            {
                                                tmp = tmp->next;
                                                switch (arg_count->child_node->type)
                                                {
                                                case ast_node::TYPE::RAW_TEXT:
                                                    tmp->type = _var::RAW_TEXT;
                                                    tmp->v_text = arg_count->child_node->value;
                                                    break;
                                                case ast_node::TYPE::STRING:
                                                    tmp->type = _var::STRING;
                                                    tmp->v_string = arg_count->child_node->value;
                                                    break;
                                                default:
                                                    tmp->type = _var::UNDECIDED;
                                                    tmp->op_list = arg_count->child_node->op;
                                                }
                                            }
                                        }
                                    }
                                    tmp_var_list = arg_var;
                                    for (tmp = arg_var; tmp->next; tmp = tmp->next)
                                        ;
                                    tmp->next = node->name_list->next;
                                }
                                else
                                    tmp_var_list = node->name_list;
                                if (found)
                                {
                                    ast_node *tmp_node = new ast_node(ast_node::TYPE::ROOT, nullptr, node->parent_node, node->name_list);
                                    tmp_node->next_node = nullptr;
                                    extend_with_var_list(tmp_node, tmp_var_list, t->func);
                                    func_call_extend(tmp_node);
                                    ast_node *tmp_end;
                                    for (tmp_end = tmp_node; tmp_end->next_node; tmp_end = tmp_end->next_node)
                                        ;
                                    tmp_end->next_node = node->next_node;
                                    node->next_node = tmp_node;
                                }
                                else
                                {
                                    extend_with_var_list(node, tmp_var_list, t->func);
                                    func_call_extend(node);
                                }
                                found = 1;
                            }
                    }
                }

            }
        }
        if (found)
            return -8;
        else
            return 0;
    }
    else
    {
        if (node->child_node)
            func_call_extend(node->child_node);
        if (node->next_node)
            func_call_extend(node->next_node);
        return 0;
    }
}

int ast_gen::property_extend(ast_node *node, int mask_num, char **mask)
{
    if (node->type == ast_node::PROPERTY)
    {
        ast_node *name_node = node->child_node;
        ast_node *value_node = name_node->next_node;
        ast_node *tmp;
        for (tmp = name_node->child_node; tmp; tmp = tmp->next_node)
        {
            if (tmp->type == ast_node::TYPE::VAR_CALL)
                var_extend(tmp, mask_num, mask);
        }
        for (value_node=name_node->next_node; value_node; value_node = value_node->next_node)
            if (value_node->type == ast_node::TYPE::OPERATION)
                op_extend(value_node, mask_num, mask);

    }
    if (node->child_node)
        property_extend(node->child_node, mask_num, mask);
    if (node->next_node)
        property_extend(node->next_node, mask_num, mask);
    return 0;
}

