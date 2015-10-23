#include "ast.h"

#include <cstdlib>
#include <cstring>

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

/*************NOT FINISHED****************/
ast_node *ast_gen::make_name(ast_node *parent_node)
{
    ast_node *name_node = new ast_node(ast_node::TYPE::NAME, nullptr, parent_node, parent_node->name_list);
    ast_node *value_node = nullptr;
    while (parsing->type == token_list_elem::RAW_TEXT || parsing->type == token_list_elem::VAR_DEFINE)
    {
        if (parsing->type == token_list_elem::RAW_TEXT)
            value_node = make_node(value_node, name_node, ast_node::TYPE::RAW_TEXT);
        else
        {
            value_node = make_node(value_node, name_node, ast_node::TYPE::OPERATION);

        }
        parsing = parsing->next;
    }
    return name_node;
}


/**************************************/

_operation *ast_gen::make_operation()
{
    _operation *result=new _operation(), *current=result;
    switch (parsing->type)
    {
    case token_list_elem::INT:
    {
        current->type = _operation::INT;
        current->i_val = atoi(parsing->content);
        break;
    }
    case token_list_elem::FLOAT:
    {
        current->type = _operation::FLOAT;
        current->f_val = atof(parsing->content);
        break;
    }
    case token_list_elem::COLOR:
    {
        current->type = _operation::COLOR;
        current->i_val = strtoul(parsing->content+1, nullptr, 16);
        break;

    }
    case token_list_elem::VAR_DEFINE:
    {
        current->type = _operation::VAR_CALL;
        current->var = make_var_call();
        break;
    }
    case token_list_elem::OPERATOR:
    {
        current->type = _operation::OPERATOR;
        current->op = *(parsing->content);
        break;
    }
    default:
        delete result;
        return nullptr;
    }
    current->next = nullptr;
    parsing = parsing->next;
    while (true)
    {
        _operation *new_current;
        switch (parsing->type)
        {
        case token_list_elem::INT:
        {
            new_current->type = _operation::INT;
            new_current->i_val = atoi(parsing->content);
            break;
        }
        case token_list_elem::FLOAT:
        {
            new_current->type = _operation::FLOAT;
            new_current->f_val = atof(parsing->content);
            break;
        }
        case token_list_elem::COLOR:
        {
            new_current->type = _operation::COLOR;
            new_current->i_val = strtoul(parsing->content+1, nullptr, 16);
            break;

        }
        case token_list_elem::VAR_DEFINE:
        {
            new_current->type = _operation::VAR_CALL;
            new_current->var = make_var_call();
            break;
        }
        case token_list_elem::OPERATOR:
        {
            new_current->type = _operation::OPERATOR;
            new_current->op = *(parsing->content);
            break;
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
}

_var_call *ast_gen::make_var_call()
{
    _var_call *var_call = new _var_call();
    var_call->undef_times = 0;
    while (parsing->type == token_list_elem::VAR_DEFINE)
    {
        ++var_call->undef_times;
        parsing = parsing->next;
    }
    var_call->name = parsing->content;
    parsing = parsing->next;
    return var_call;
}

int ast_gen::sub_block_parser(ast_node *content_node)
{
    ast_node *current_node = nullptr;
    while (parsing->type != token_list_elem::BLOCK_END)
    {
        switch (parsing->type)
        {
        case token_list_elem::RAW_TEXT:
        {
            ast_node *new_node = make_node(current_node, content_node, ast_node::TYPE::RAW_TEXT);
            new_node->value = parsing->content;
            current_node = new_node;
            parsing = parsing->next;
            break;
        }
        case token_list_elem::SELECTOR_NAME:
        {

            parsing = parsing->next;
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
            parsing = parsing->next;
            if (parsing->type != token_list_elem::RAW_TEXT)
                return -3;
            _var *new_name = new _var(_var::UNDECIDED);
            new_name->name = parsing->content;
            new_name->next = current_node->name_list->next;
            current_node->name_list->next = new_name;
            parsing = parsing->next;
            if (parsing->type != token_list_elem::VAR_ASSIGN)
                return -4;
            parsing = parsing->next;
            if (parsing->type == token_list_elem::STRING)
            {
                new_name->string_with_var = parsing->content;
                new_name->type = _var::STRING;
            }
            else
            {
                new_name->op_list = make_operation();
                new_name->type = _var::UNDECIDED;
            }
            _var *t1, *t2;
            for (t1=new_name, t2=new_name->next; !(t2->type == _var::NEW_FLAG); t1=t1->next, t2=t2->next)
            {
                if (!strcmp(t2->name, new_name->name))
                {
                    if (state == STATE_NORMAL)
                    {
                        t1->next = t2->next;
                        delete t2;
                    }
                    else
                    {
                        current_node->name_list->next = new_name->next;
                        delete new_name;
                    }
                }
            }
            break;

        }
        }
    }
    return 0;
}

int ast_gen::first_step()
{
    return sub_block_parser(root_node);
}

int ast_gen::second_step()
{
    return 0;
}