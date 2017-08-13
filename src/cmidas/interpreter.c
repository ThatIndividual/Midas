#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>

static struct val bin_op(struct tok*, struct val, struct val);
static struct val un_op(struct tok*, struct val);

static void print_val(struct val);

struct val
val_new(struct tok* tok)
{
    struct val val;

    switch (tok->type)
    {
        case TOK_INTEGER:
            val.type = VAL_INTEGER;
            // XXX this will silently fail for large enough numbers
            val.data.as_long = atol(tok->lexeme);
            break;

        default:
            printf("Interpreter error:\n"
                   "Tried to enval un unrecognized token type\n");
            exit(0);
    }

    return val;
}

void
execute(struct stm* stm)
/* Take a statement and produce a side effect.                               */
{
    switch (stm->type)
    {
        case STM_BLOCK:
            ;
            struct stmlist* list = stm->data.block;
            struct stmnode* node = list->nil;
            for (int i = 0; i < list->length; ++i)
            {
                node = node->next;
                execute(node->data);
            }
            break;

        case STM_EXPR_STMT:
            /* Evaluate and discard */
            evaluate(stm->data.expr.exp);
            break;

        case STM_PRINT:
            ;
            struct val val = evaluate(stm->data.print.exp);
            print_val(val);
            break;
    }
}

static void
print_val(struct val val)
{
    switch (val.type)
    {
        case VAL_INTEGER:
            printf("%li\n", val.data.as_long);
            break;

        /* XXX case VAL_DOUBLE */

        /* XXX case VAL_STRING */
    }
}

struct val
evaluate(struct exp* exp)
/* Take an expression and output a value                                     */
{
    switch (exp->type)
    {
        case EXP_BINARY:
            ;
            struct val left = evaluate(exp->data.binary.left);
            struct val right = evaluate(exp->data.binary.right);
            return bin_op(exp->data.binary.op, left, right);

        case EXP_UNARY:
            ;
            struct val operand = evaluate(exp->data.unary.exp);
            return un_op(exp->data.unary.op, operand);

        case EXP_GROUP:
            return evaluate(exp->data.group.exp);

        case EXP_LITERAL:
            return val_new(exp->data.literal);

        default:
            printf("Interpreter error:\n"
                   "Tried to evaluate unexpected expression type.\n");
            exit(0);
    }
}

static struct val
bin_op(struct tok* tok, struct val left, struct val right)
{
    switch (tok->type)
    {
        case TOK_PLUS:
            left.data.as_long = left.data.as_long + right.data.as_long;
            return left;

        case TOK_MINUS:
            left.data.as_long = left.data.as_long - right.data.as_long;
            return left;

        case TOK_STAR:
            left.data.as_long = left.data.as_long * right.data.as_long;
            return left;

        case TOK_SLASH:
            left.data.as_long = left.data.as_long / right.data.as_long;
            return left;

        default:
            printf("Interpreter error:\n"
                   "Tried to apply an unexpected binary operation.\n");
            exit(0);
    }
}

static struct val
un_op(struct tok* tok, struct val operand)
{
    switch (tok->type)
    {
        case TOK_MINUS:
            operand.data.as_long = - operand.data.as_long;
            return operand;

        /* XXX case TOK_BANG */

        default:
            printf("Interpreter error:\n"
                   "Tried to apply an unexpected unary operation.\n");
            exit(0);
    }
}

