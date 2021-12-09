/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file (standard input) instead.
 */

#include <check.h>

#include <stdlib.h>
#define TEST
#include "../src/fruits.h"


GAME game;

/* stubs and mocks */
POINT *snake_part_is_on(SNAKE *snake, int posy, int posx) {
    return NULL;
}

int check_extended_border_collision(GAME* game, int cury, int curx) {
    return 0;
}


void setup_fruits(unsigned int num_fruits_to_grow) {
    for(int i = 0; i < num_fruits_to_grow; i++) {
        grow_fruit(&game);
    }
}

void check_fruit_position(const FRUIT * fruit, int expected_y, int expected_x) {
    if(fruit == NULL) return;

    ck_assert_int_eq(fruit->point.y, expected_y);
    ck_assert_int_eq(fruit->point.x, expected_x);
    ck_assert_int_ne(fruit->point.ch, '\0');
}

void check_fruit_array(const FRUIT * fruit) {
    if(fruit->point.ch == '\0') {
        ck_assert_int_eq(fruit->point.y, 0);
        ck_assert_int_eq(fruit->point.x, 0);
    } else {
        const FRUIT * hopefully_the_same_fruit = get_fruit_on(&game, fruit->point.y, fruit->point.x);
        ck_assert_ptr_eq(hopefully_the_same_fruit, fruit);

        check_fruit_position(hopefully_the_same_fruit, fruit->point.y, fruit->point.x);
        check_fruit_position(hopefully_the_same_fruit, fruit->point.y, fruit->point.x);
    }
}

int count_fruits() {
    int fruit_count = 0;
    for(int y = 0; y < game.rows; y++) {
        for(int x = 0; x < game.columns; x++) {
            const FRUIT* fruit = get_fruit_on(&game, y, x);
            if(fruit != NULL) fruit_count++;
            check_fruit_position(fruit, y, x);
        }
    }
    return fruit_count;
}

int eat_fruits(int count) {
    int eaten = 0;
    int fruits_available = count_fruits();
    int actual_count = fruits_available < count ? fruits_available : count;

    while(eaten < actual_count && game.fruits.allocated > 0) {
        unsigned int idx = rand() % game.fruits.allocated;
        FRUIT * fruit = &game.fruits.fruits[idx];

        if(is_fruit_used(fruit)) {
            check_fruit_array(fruit);
            kill_fruit(&game.fruits, fruit);
            check_fruit_array(fruit);
            eaten++;
        }

    }
    return eaten;
}


/** setup / teardown **/

void setup() {
    srand(time(NULL));
    game.rows = 9;
    game.columns = 11;
}

void teardown() {
   kill_fruits(&game.fruits);
}



START_TEST(test_grow_fruit)
{
    const int num_fruits_to_grow = _i;
    fprintf(stderr, "test_grow_fruit %d", _i);
    setup_fruits(num_fruits_to_grow);
    ck_assert_int_eq(count_fruits(), num_fruits_to_grow);

}
END_TEST

START_TEST(test_grow_too_much_fruit)
{
    const int num_fruits_to_grow = _i;
    fprintf(stderr, "test_grow_too_much_frit %d", _i);
    setup_fruits(num_fruits_to_grow);
    ck_assert_int_eq(count_fruits(), num_fruits_to_grow);


}
END_TEST

START_TEST(test_grow_fruit_full_field_eat_half)
{
    const int num_fruits_to_grow = 100;
    const int num_fruits_to_eat = 50;

    setup_fruits(num_fruits_to_grow);
    ck_assert_int_eq(count_fruits(), num_fruits_to_grow);

    int fruits_eaten = eat_fruits(num_fruits_to_eat);

    ck_assert_int_eq(num_fruits_to_eat, fruits_eaten);
    ck_assert_int_eq(count_fruits(), num_fruits_to_grow - num_fruits_to_eat);

}
END_TEST

START_TEST(test_grow_and_eat_alternating)
{

    int num_to_grow = 1;
    int num_to_eat = 1;

    setup_fruits(num_to_grow);




}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Snake Fruits");
    TCase *tc1_1 = tcase_create("Growing Fruits");
    SRunner *sr = srunner_create(s1);
    int nf;

    /* User-specified pre-run code */
    tcase_add_checked_fixture(tc1_1, setup, teardown);

    suite_add_tcase(s1, tc1_1);
    tcase_add_loop_test(tc1_1, test_grow_fruit, 0, 99);
    tcase_add_loop_test(tc1_1, test_grow_too_much_fruit, 100, 110);
    tcase_add_test(tc1_1, test_grow_fruit_full_field_eat_half);
    tcase_add_test(tc1_1, test_grow_and_eat_alternating);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}