/*
 * =====================================================================================
 *
 *       Filename:  busy_job.c
 *
 *    Description:  创新工场每年。。。
 *
 *        Version:  1.0
 *        Created:  09/07/2012 07:00:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  hmgle (), dustgle@gmail.com
 *        Company:  NULL
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#ifndef MAX_LINE
#define MAX_LINE        1024
#endif

#ifndef SUCCESS
#define SUCCESS         0
#endif

#ifndef FAIL
#define FAIL            -1
#endif

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#define MAX_SEL_PRJ     3
#define MEM_APY_RAT     3
#define DEBUG_PRINT     0
#define debug_print(fmt, ...) \
    do { if (DEBUG_PRINT) fprintf(stderr, "-------%s: %d: %s():---" fmt "----\n", \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);} while (0)

typedef int BOOL;

typedef struct st_mate_info {
    int32_t no;
    int32_t *pprj;
} MATE_INFO;

typedef struct st_prj_info {
    int32_t no;
    int32_t *pmate;
} PRJ_INFO;

static int lefttrim(char *s)
{
    int i;
    int j;
    int n;
    int tmp;

    n = strlen(s);
    for (i = 0; i < n; i++)
        if (s[i] != ' ' && s[i] != '\t')
            break;
    tmp = i;
    j = 0;
    while (s[i])
        s[j++] = s[i++];

    return n - tmp;
} /* static int lefttrim(char *s) */

static int32_t read_first_num_from_string(char *src)
{
    int i = 0;
    int32_t ret = 0;

    lefttrim(src);
    while (src[i] >= '0' && src[i] <= '9') {
        ret = ret * i + (src[i] - '0');
        i++;
    }
    return ret;
} /* static int32_t read_first_num_from_string(char *scr) */

static int read_second_num_from_string(char *src)
{
    int ret = 0;
    char *tmp_p = src;

    lefttrim(src);
    while (!isgraph(*tmp_p))
        tmp_p++;
    tmp_p++;
    ret = read_first_num_from_string(tmp_p);

    return ret;
} /* static int read_second_num_from_string(char *src) */

static int32_t get_mate_num(FILE *input)
{
    char one_line[MAX_LINE];
    int32_t num = 0;
    int32_t tmp_num;
    int ret;

    ret = fseek(input, 0L, SEEK_SET);
    if (ret) {
        debug_print("fseek failed with %#x! ", ret);
        return ret;
    }
    one_line[MAX_LINE - 1] = '\0';
    while (fgets(one_line, MAX_LINE - 1, input)) {
        lefttrim(one_line);
        if (one_line[0] != '0') {
            tmp_num = read_first_num_from_string(one_line);
            if (tmp_num > num)
                num = tmp_num;
        } else 
            break;
    }
    return num;
} /* static int read_second_num_from_string(char *src) */

static int32_t get_prj_num(FILE *input)
{
    char one_line[MAX_LINE];
    int32_t num = 0;
    int32_t tmp_num;
    int ret;

    ret = fseek(input, 0L, SEEK_SET);
    if (ret) {
        debug_print("fseek failed with %#x! ", ret);
        return ret;
    }
    one_line[MAX_LINE - 1] = '\0';
    while (fgets(one_line, MAX_LINE - 1, input)) {
        lefttrim(one_line);
        if (one_line[0] != '0') {
            tmp_num = read_second_num_from_string(one_line);
            if (tmp_num > num)
                num = tmp_num;
        } else 
            break;
    }

    return num;
} /* static int32_t get_prj_num(FILE *input) */

int32_t mate_prj_fill_from_input(int32_t *pmate_prj_list, int32_t n, FILE *input)
{
    int ret;
    int32_t total = 0;
    int32_t tmp_int;
    int32_t i;
    char one_line[MAX_LINE];

    ret = fseek(input, 0L, SEEK_SET);
    if (ret) {
        debug_print("fseek failed with %#x! ", ret);
        return ret;
    }
    one_line[MAX_LINE - 1] = '\0';

    i = 0;
    while (fgets(one_line, MAX_LINE - 1, input) && total < MAX_SEL_PRJ) {
        lefttrim(one_line);
        if (one_line[0] != '0') {
            tmp_int = read_first_num_from_string(one_line);
            if (tmp_int == n) {
                tmp_int = read_second_num_from_string(one_line);
                pmate_prj_list[++i] = tmp_int;
                total++;
            }
        } else
            break;
    }

    return total;
} /* void mate_prj_fill_from_input(int32_t *pmate_prj_list, int32_t n, FILE *input) */

static int cat_list(int32_t *list, const int32_t n, const int32_t max)
{
    int32_t i = 0;
    while (*list && i < max) {
        list++;
    }
    if (i < max) {
        *list = n;
        return SUCCESS;
    } else
        return FAIL;
} /* static int cat_list(int32_t *list, const int32_t n, const int32_t max) */

static void scan_input_to_fill_mate_prj(MATE_INFO **ppmate_prj_list, const int32_t n, FILE *input)
{
    int ret;
    char one_line[MAX_LINE];
    int32_t tmp_num;
    int32_t tmp_num2;

    ret = fseek(input, 0L, SEEK_SET);
    if (ret) {
        debug_print("fseek failed with %#x! ", ret);
        exit(errno);
    }
    one_line[MAX_LINE - 1] = '\0';
    while (fgets(one_line, MAX_LINE - 1, input)) {
        lefttrim(one_line);
        if (one_line[0] != '0') {
            tmp_num = read_first_num_from_string(one_line);
            debug_print("tmp_num is %d ", tmp_num);
            tmp_num2 = read_second_num_from_string(one_line);
            debug_print("tmp_num2 is %d ", tmp_num2);
            if (tmp_num > 0 && tmp_num2 > 0)
                cat_list(ppmate_prj_list[tmp_num - 1]->pprj, tmp_num2, MAX_SEL_PRJ);
        } else
            break;
    }
    return;
} /* static void scan_input_to_fill_mate_prj(MATE_INFO **ppmate_prj_list, int32_t n, FILE *input) */

static int find_next_zero(int32_t *list, int32_t **p, const int32_t max)
{
    int32_t *pl = list + 1;
    int i = 1;
    *p = NULL;
    while (*pl && i < max) {
        pl++;
        i++;
    }
    if (i < max) {
        *p = pl;
        return i;
    } else
        return FAIL;
} /* static int find_next_zero(int32 *list, int32_t **p, int32_t max) */

static BOOL check_ins_able(const PRJ_INFO **ppprj_mate_list, const int32_t row, const int32_t col, const int32_t num, const int32_t prj_num)
{
    int i;
    for (i = 0; i < row; i++) {
        if (ppprj_mate_list[i]->pmate[col] == num)
            return FALSE;
    }
    for (i = row + 1; i < prj_num; i++) {
        if (ppprj_mate_list[i]->pmate[col] == num)
            return FALSE;
    }
    return TRUE;
} /* static BOOL check_ins_able(const PRJ_INFO **ppprj_mate_list, int32_t row, int32_t col, int32_t num, int32_t prj_num) */

static void mate_ins_prj(PRJ_INFO **ppprj_mate_list, const int32_t prj_num, const MATE_INFO mate_prj_list, const int32_t max_mate)
{
    int32_t mate_no;
    int32_t prj_no;
    int32_t *pprj;
    int32_t *pmate;
    int32_t col;
    int32_t tmp_col;
    int32_t total = 0;

    mate_no = mate_prj_list.no;
    pprj = mate_prj_list.pprj;

    while (*pprj && total < MAX_SEL_PRJ) {
        prj_no = *pprj;
        debug_print("prj_no is %d ", prj_no);

        pmate = ppprj_mate_list[prj_no - 1]->pmate;
        col = 0;
#if 0
        do {
            tmp_col = find_next_zero(pmate, &pmate, max_mate);
            if (tmp_col == FAIL) {
                debug_print("find_next_zero failed!");
                exit(1);
            }
            col += tmp_col;

            ret = check_ins_able((const PRJ_INFO **)ppprj_mate_list, prj_no - 1, col, mate_no, prj_num);
        } while (!ret);
#else
        while (*(ppprj_mate_list[prj_no - 1]->pmate + col) != 0 || \
                !check_ins_able((const PRJ_INFO **)ppprj_mate_list, prj_no - 1, col, mate_no, prj_num)) {
            tmp_col = find_next_zero(pmate, &pmate, max_mate);
            if (tmp_col == FAIL) {
                debug_print("find_next_zero failed!");
                exit(1);
            }
            col += tmp_col;
        }
#endif
        debug_print("--prj_no is %d ", prj_no);
        debug_print("col is %d ", col);
        debug_print("mate_no is %d ", mate_no);
        *(ppprj_mate_list[prj_no - 1]->pmate + col) = mate_no;

        pprj++;
        total++;
    } /* while (*pprj && total < MAX_SEL_PRJ)  */

    return;
} /* static void mate_ins_prj(PRJ_INFO **ppprj_mate_list, int32_t prj_num, MATE_INFO mate_prj_list, const int32_t max_mate) */

static char *del_tail(char *str, const int32_t end)
{
    int i = end - 1;
    /*
     * find last not zero num
     */
    while ((str[i] > '9' || str[i] <= '0') && i > 0)
        i--;
    str[++i] = '\0';
    return str;
} /* static del_tail(char *str) */

static char *read_prj_info(char *dest, PRJ_INFO **ppprj_mate_list, int32_t row, int32_t max_num)
{
    int32_t i;
    int ret;
    char *tmp;

    ret = sprintf(dest, "%d ", ppprj_mate_list[row]->no);
    tmp = dest + ret;
    for (i = 0; i < max_num; i++) {
        ret = sprintf(tmp, "%d ", *(ppprj_mate_list[row]->pmate + i));
        tmp += ret;
    }

    /*
     * trim
     */
    del_tail(dest, max_num * 3);
    return dest;
} /* static char *read_prj_info(char *dest, PRJ_INFO **ppprj_mate_list, int32_t row, int32_t max_num) */

static int32_t get_prj_long(PRJ_INFO *pprj_mate_list, const int32_t max_num)
{
    int32_t ret = 0;
    int32_t i;

    for (i = 0; i < max_num; i++)
        if (*(pprj_mate_list->pmate + i) > 0)
            ret++;

    return ret;
} /* static int32_t get_prj_long(PRJ_INFO *pprj_mate_list, const int32_t max_num) */

static int32_t get_prj_max_long(PRJ_INFO **ppprj_mate_list, const int32_t prj_num, const int32_t max_num)
{
    int32_t ret = 0;
    int32_t tmp;
    int32_t i;

    for (i = 0; i < prj_num; i++) {
        tmp = get_prj_long(ppprj_mate_list[i], max_num);
        if (tmp > ret)
            ret = tmp;
    }

    return ret;
} /* static int32_t get_prj_max_long(PRJ_INFO **ppprj_mate_list, const int32_t prj_num, const int32_t max_num) */

int main(int argc, char **argv)
{
    FILE *input;
    FILE *output;

    int32_t mate_num;
    int32_t prj_num;

    int ret;
    int i;
    int j;

    if (argc < 2) {
        fprintf(stderr, "usage: %s inputfile [outfile]\n", argv[0]);
        return 1;
    }

    input = fopen(argv[1], "r");
    if (!input) {
        debug_print("fopen failed ");
        return errno;
    }
    if (argc < 3)
        output = fopen("output.txt", "w");
    else
        output = fopen(argv[2], "w");

    if (!output) {
        debug_print("fopen failed ");
        return errno;
    }

    mate_num = get_mate_num(input);
    prj_num  = get_prj_num(input);


    /*
     * apply mem
     */
    MATE_INFO **ppmate_prj_list;
    ppmate_prj_list = (MATE_INFO **)malloc(mate_num * sizeof(MATE_INFO *));
    if (!ppmate_prj_list) {
        debug_print("malloc failed! ");
        return errno;
    }

    for (i = 0; i < mate_num; i++) {
        ppmate_prj_list[i] = (MATE_INFO *)malloc(sizeof(MATE_INFO));
        if (!ppmate_prj_list[i]) {
            debug_print("malloc failed! ");
            return errno;
        }
        ppmate_prj_list[i]->no = i + 1;

        ppmate_prj_list[i]->pprj = (int32_t *)malloc(MAX_SEL_PRJ * sizeof(int32_t));
        if (!ppmate_prj_list[i]->pprj) {
            debug_print("malloc failed! ");
            return errno;
        }
        memset(ppmate_prj_list[i]->pprj, 0, MAX_SEL_PRJ * sizeof(int32_t));
    }

    PRJ_INFO **ppprj_mate_list;
    ppprj_mate_list = (PRJ_INFO **)malloc(prj_num * sizeof(PRJ_INFO *));
    if (!ppprj_mate_list) {
        debug_print("malloc failed! ");
        return errno;
    }

    for (i = 0; i < prj_num; i++) {
        ppprj_mate_list[i] = (PRJ_INFO *)malloc(sizeof(PRJ_INFO));
        if (!ppprj_mate_list[i]) {
            debug_print("malloc failed! ");
            return errno;
        }
        ppprj_mate_list[i]->no = i + 1;

        ppprj_mate_list[i]->pmate = (int32_t *)malloc(mate_num * MEM_APY_RAT * sizeof(int32_t));
        if (!ppprj_mate_list[i]->pmate) {
            debug_print("malloc failed! ");
            return errno;
        }
        memset(ppprj_mate_list[i]->pmate, 0, mate_num * MEM_APY_RAT * sizeof(int32_t));
    } /* for (i = 0; i < prj_num; i++) */

    /*
     * init data
     */
#if 0
    for (i = 0; i < mate_num; i++) {
        mate_prj_fill_from_input(ppmate_prj_list[i], i + 1, input);
    }
#else
    scan_input_to_fill_mate_prj(ppmate_prj_list, mate_num, input);
#endif
    debug_print(" ppmate_prj_list 0 0 is  %d", *(ppmate_prj_list[0]->pprj));
    debug_print(" ppmate_prj_list 0 1 is  %d", *(ppmate_prj_list[0]->pprj + 1));
    debug_print(" ppmate_prj_list 0 2 is  %d", *(ppmate_prj_list[0]->pprj + 2));
    debug_print(" ppmate_prj_list 1 0 is  %d", *(ppmate_prj_list[1]->pprj));
    debug_print(" ppmate_prj_list 1 1 is  %d", *(ppmate_prj_list[1]->pprj + 1));
    debug_print(" ppmate_prj_list 1 2 is  %d", *(ppmate_prj_list[1]->pprj + 2));
    debug_print(" ppmate_prj_list 2 0 is  %d", *(ppmate_prj_list[2]->pprj));
    debug_print(" ppmate_prj_list 2 1 is  %d", *(ppmate_prj_list[2]->pprj + 1));
    debug_print(" ppmate_prj_list 2 2 is  %d", *(ppmate_prj_list[2]->pprj + 2));

    debug_print("\n");
    /*
     * insert data
     */
    for (i = 0; i < mate_num; i++) {
        mate_ins_prj(ppprj_mate_list, prj_num, *ppmate_prj_list[i], mate_num * MEM_APY_RAT);
    }

    debug_print(" ppprj_mate_list 0 0 is  %d", *(ppprj_mate_list[0]->pmate));
    debug_print(" ppprj_mate_list 0 1 is  %d", *(ppprj_mate_list[0]->pmate + 1));
    debug_print(" ppprj_mate_list 0 2 is  %d", *(ppprj_mate_list[0]->pmate + 2));

    debug_print(" ppprj_mate_list 1 0 is  %d", *(ppprj_mate_list[1]->pmate));
    debug_print(" ppprj_mate_list 1 1 is  %d", *(ppprj_mate_list[1]->pmate + 1));
    debug_print(" ppprj_mate_list 1 2 is  %d", *(ppprj_mate_list[1]->pmate + 2));

    debug_print(" ppprj_mate_list 2 0 is  %d", *(ppprj_mate_list[2]->pmate));
    debug_print(" ppprj_mate_list 2 1 is  %d", *(ppprj_mate_list[2]->pmate + 1));
    debug_print(" ppprj_mate_list 2 2 is  %d", *(ppprj_mate_list[2]->pmate + 2));

    /*
     * optimization
     */

    /* **********************************************************
     * %^*(*(*(*9^^%*&**(*)(((((*))))))))
     * &^**&(*)(^(^(&(*)*)^%@#$%^&*()^&*()_(*&@#$%^&"><%&*(*))))
     * (&*((*((*^^&*&(@#$%^&*($%^&*(#$%^&))))))))
     * **********************************************************
     */

    /*
     * save outputfile
     */
    ret = fseek(output, 0L, SEEK_SET);
    if (ret) {
        debug_print("fseek failed with %#x! ", ret);
        return ret;
    }
    
#if 0
    char one_line[MAX_LINE];
    for (i = 0; i < prj_num; i++) {
        read_prj_info(one_line, ppprj_mate_list, i, mate_num * MEM_APY_RAT);
        fprintf(output, "%s\n", one_line);
    }
#else
    int32_t prj_max_c;
    prj_max_c = get_prj_max_long(ppprj_mate_list, prj_num, mate_num * MEM_APY_RAT);
    for (i = 0; i < prj_num; i++) {
        for (j = 0; j < prj_max_c; j++) {
            fprintf(output, "%3d ", *(ppprj_mate_list[i]->pmate + j));
        }
        fprintf(output, "\n");
    }
#endif

    /*
     * realse
     */
    for (i = 0; i < prj_num; i++) {
        if (ppprj_mate_list[i]) {
            free(ppprj_mate_list[i]);
            ppprj_mate_list[i] = NULL;
        }
    }
    free(ppprj_mate_list);
    ppprj_mate_list = NULL;

    for (i = 0; i < mate_num; i++) {
        if (ppmate_prj_list[i]) {
            free(ppmate_prj_list[i]);
            ppmate_prj_list[i] = NULL;
        }
    }
    free(ppmate_prj_list);
    ppmate_prj_list = NULL;
    fclose(output);
    fclose(input);
    return 0;
} /* int main(int argc, char **argv) */

