#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../toPascalCase.hpp"


TEST(test_toPascalCase_empty) {
	string input = "";
	string expected = "";
	string actual = toPascalCase(input);
	assert(actual == expected && "Empty string");
}

TEST(test_toPascalCase_single_word) {
	string input = "hello";
	string expected = "Hello";
	string actual = toPascalCase(input);
	assert(actual == expected && "Single word lowercase");
}

TEST(test_toPascalCase_already_pascal) {
	string input = "HelloWorld";
	string expected = "HelloWorld";
	string actual = toPascalCase(input);
	assert(actual == expected && "Already PascalCase");
}

TEST(test_toPascalCase_hyphen_separated) {
	string input = "cpp-tui";
	string expected = "CppTui";
	string actual = toPascalCase(input);
	assert(actual == expected && "Hyphen separated (cpp-tui)");
}

TEST(test_toPascalCase_underscore_separated) {
	string input = "some_example";
	string expected = "SomeExample";
	string actual = toPascalCase(input);
	assert(actual == expected && "Underscore separated (some_example)");
}

TEST(test_toPascalCase_dot_separated) {
	string input = "my.lib";
	string expected = "MyLib";
	string actual = toPascalCase(input);
	assert(actual == expected && "Dot separated (my.lib)");
}

TEST(test_toPascalCase_multiple_hyphens) {
	string input = "my--lib";
	string expected = "MyLib";
	string actual = toPascalCase(input);
	assert(actual == expected && "Multiple consecutive hyphens");
}

TEST(test_toPascalCase_mixed_separators) {
	string input = "some_lib-name.test";
	string expected = "SomeLibNameTest";
	string actual = toPascalCase(input);
	assert(actual == expected && "Mixed separators (some_lib-name.test)");
}

TEST(test_toPascalCase_single_char) {
	string input = "a";
	string expected = "A";
	string actual = toPascalCase(input);
	assert(actual == expected && "Single character");
}

TEST(test_toPascalCase_uppercase_input) {
	string input = "CPP-TUI";
	string expected = "CPPTUI";
	string actual = toPascalCase(input);
	assert(actual == expected && "Uppercase input preserved");
}

#endif
