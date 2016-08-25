//
// Burst
// Include/Parser/AST/Nodes/BurstLiteralExpressionNode.h
//
#ifndef __BURST_AST_LITERAL_EXPRESSION_NODE_H__
#define __BURST_AST_LITERAL_EXPRESSION_NODE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <assert.h>

#include "BurstErrorCodes.h"
#include "BurstErrorMessages.h"

#include "BurstASTNode.h"

struct burstLiteralExpressionNode;
struct burstLiteralExpressionNode
{
    // TODO: Value Type
    
    char *pValueString;
    
    int (*destroy)
    (
        struct burstLiteralExpressionNode *pNode
    );
};
typedef struct burstLiteralExpressionNode BurstLiteralExpressionNode;

int literal_expression_node_create
(
    char *pExpressionValue,             // IN
    BurstLiteralExpressionNode **ppNode // OUT
);

int literal_expression_node_destroy
(
    BurstLiteralExpressionNode *pNode // IN
);

#endif