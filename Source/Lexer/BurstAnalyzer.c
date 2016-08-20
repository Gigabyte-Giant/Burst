//
// Burst
// Source/Lexer/BurstAnalyzer.c
//
#include "Lexer/BurstAnalyzer.h"

int analyzer_create
(
    BurstTokenArray *pInputTokens,
    BurstTokenRegistry *pTokenRegistry,
    BurstAnalyzer **ppAnalyzer
)
{
    if (NULL != (*ppAnalyzer))
        return BURST_FAIL;
    
    if (NULL == ((*ppAnalyzer) = (BurstAnalyzer *) malloc(sizeof(BurstAnalyzer))))
        return BURST_FAIL;
    
    (*ppAnalyzer)->pInputTokens   = pInputTokens;
    (*ppAnalyzer)->pOutputTokens  = NULL;
    (*ppAnalyzer)->pTokenRegistry = pTokenRegistry;
    
    token_array_create(&((*ppAnalyzer)->pOutputTokens));
    
    return BURST_SUCCESS;
}

int analyzer_run
(
    BurstAnalyzer *pAnalyzer
)
{
    BurstToken *pLastInputToken = NULL;
    BurstToken *pCurrentInputToken = NULL;
    
    int outputTokenType = BURST_UNKNOWN_TOKEN;
    char *pOutputTokenValue = NULL;
    size_t outputTokenValueLength = 0x0;
    
    if (NULL == pAnalyzer)
        return BURST_FAIL;
    
    if (NULL == pAnalyzer->pInputTokens || NULL == pAnalyzer->pOutputTokens)
        return BURST_FAIL;
    
    for (int inputTokenIndex = 0x0; inputTokenIndex <
        pAnalyzer->pInputTokens->tokenCount; ++inputTokenIndex)
    {
        pCurrentInputToken = token_array_at(inputTokenIndex,
            pAnalyzer->pInputTokens);
        
        if (NULL == pOutputTokenValue)
            pOutputTokenValue = (char *) malloc(sizeof(char));
        
        if (outputTokenType == pCurrentInputToken->type)
        {
            pOutputTokenValue = (char *) realloc(pOutputTokenValue, (
                sizeof(char) * (outputTokenValueLength + 0x1))
            );
            
            *(pOutputTokenValue + outputTokenValueLength) = (
                pCurrentInputToken->charValue
            );
            
            outputTokenValueLength++;
        }
        else
        {
            if (BURST_UNKNOWN_TOKEN != outputTokenType &&
                BURST_WHITESPACE_TOKEN != outputTokenType)
            {
                BurstToken *pTokenRegistryResult = NULL;
                BurstToken *pOutputToken = NULL;
                
                pOutputTokenValue = (char *) realloc(pOutputTokenValue, (
                    sizeof(char) * (outputTokenValueLength + 0x1)
                ));
                
                *(pOutputTokenValue + outputTokenValueLength) = '\0';
                
                if (NULL != (pTokenRegistryResult = token_registry_get_s(
                    pOutputTokenValue, pAnalyzer->pTokenRegistry)))
                    outputTokenType = pTokenRegistryResult->type;
                else
                {
                    switch (outputTokenType)
                    {
                        case BURST_LETTER_TOKEN:
                            outputTokenType = BURST_IDENTIFIER_TOKEN;
                            break;
                        
                        default:
                            break;
                    }
                }
                
                token_create_s(strdup(pOutputTokenValue), outputTokenType,
                    &pOutputToken);
                token_array_add(pOutputToken, pAnalyzer->pOutputTokens);
                
                // THIS IS JUST A TEST, BUT I REALLY HOPE IT WORKS!
                printf("%s - %04x\n", pOutputToken->pStringValue,
                    pOutputToken->type);
            }
            
            free(pOutputTokenValue);
            pOutputTokenValue = (char *) calloc(0x1, sizeof(char));
            
            memcpy(pOutputTokenValue, &pCurrentInputToken->charValue, sizeof(char));
            
            outputTokenValueLength = 0x1;
            outputTokenType = pCurrentInputToken->type;
        }
        
        pLastInputToken = pCurrentInputToken;
    }
    
    if (NULL != pOutputTokenValue)
        free(pOutputTokenValue);
    
    return BURST_SUCCESS;
}

int analyzer_destroy
(
    BurstAnalyzer *pAnalyzer
)
{
    if (NULL == pAnalyzer)
        return BURST_FAIL;
    
    if (NULL != pAnalyzer->pOutputTokens)
        token_array_destroy(pAnalyzer->pOutputTokens);
    
    free(pAnalyzer);
    
    return BURST_SUCCESS;
}