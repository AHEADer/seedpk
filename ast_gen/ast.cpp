#include "ast.h"

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
            value_node = make_node(value_node, name_node, ast_node::TYPE::VAR_CALL);

        }
        parsing = parsing->next;
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
        {
            ast_node *new_node = make_node(current_node, content_node, ast_node::TYPE::RAW_TEXT);
            new_node->value = parsing->content;
            current_node = new_node;
            break;
        }
        case token_list_elem::SELECTOR_NAME:
        {
            ast_node *selector_node = make_node(current_node, content_node, ast_node::TYPE::SELECTOR);
            ast_node *name_node = new ast_node(ast_node::TYPE::NAME, nullptr, selector_node, selector_node->name_list);
            name_node->value = parsing->content;
            selector_node->child_node = name_node;
            parsing = parsing->next;
            while (parsing->type == token_list_elem::SELECTOR_NAME)
            {
                name_node = new ast_node(ast_node::TYPE::NAME, name_node, selector_node, name_node->name_list);
                name_node->value = parsing->content;
                parsing = parsing->next;
            }
            if (parsing->type == token_list_elem::TYPE::BLOCK_BEGIN)
            {
                _var *new_name_list = new _var(1);
                new_name_list->next = selector_node->name_list;
                ast_node *new_content_node = new ast_node(ast_node::TYPE::CONTENT, name_node, selector_node, new_name_list);
                int flag = sub_block_parser(new_content_node);
                if (flag)
                    return flag;
            }
            else
                return -1;
            current_node = selector_node;
            break;
        }
        case token_list_elem::BLOCK_BEGIN:
            return -2;
            break;
        case token_list_elem::SEPARATOR:
            continue;
        case token_list_elem::VAR_DEFINE:
        {
            parsing = parsing->next;
            if (parsing->type != token_list_elem::VAR_NAME)
                return -3;
            _var *new_name = new _var(0);
            new_name->name = parsing->content;
            new_name->next = current_node->name_list->next;
            current_node->name_list->next = new_name;
            parsing = parsing->next;
            if (parsing != token_list_elem::VAR_ASSIGN)
                return -4;
            parsing = parsing->next;
            if (parsing->type == token_list_elem::STRING)
                new_name->string_with_var = parsing->content;
            else
                new_name->op_list = make_operation();
            _var *t1, *t2;
            for (t1=new_name, t2=new_name->next; !(t2->new_flag); t1=t1->next, t2=t2->next)
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
        parsing = parsing->next;
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
