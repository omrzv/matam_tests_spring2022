from ex5 import names_of_registered_students, enrollment_numbers, courses_for_lecturers
import json
import unittest

input_json_path1 = "students_database.json"
database_directory_path1 = "semesters_databases"

input_json_path2 = "students_database2.json"
database_directory_path2 = "semesters_databases2"


class Test(unittest.TestCase):

    def test1(self):
        """
        this test for the first function - names_of_registered_students.

        """

        correct_answer = ["Mohamed Gramne", "Mhameed",
                          "Efrat Levkovizh", "Yazeed Falah"]

        result = names_of_registered_students(
            input_json_path1, "Introduction to Systems Programming")

        correct_answer.sort()
        result.sort()

        self.assertListEqual(correct_answer, result, "Error: the output of the function   \
                                                  names_of_registered_students        \
                                                  doesn't match the expected one.")

    def test2(self):
        """
        this test for the second function - enrollment_number.
        """

        expected_output_file = "expected_output_test2_enrollment_numbers.txt"
        enrollment_numbers(input_json_path1, "test2.out")
        with open("test2.out") as file_to_test:
            data_to_test = list(file_to_test)

        with open(expected_output_file) as expected_file:
            expected_data = list(expected_file)

        self.assertListEqual(data_to_test, expected_data, "Error: the content of the file created by   \
                                              the fucntion - enrollment_number     \
                                              doesn't match the expected one.")

    def test3(self):
        """
        this test for the third function - courses_for_lecturers.
        """
        expected_output_file = "expected_output_test3_courses_for_lecturers.json"
        courses_for_lecturers(database_directory_path1, "test3_output.json")

        with open("test3_output.json") as file_to_test:
            data_to_test = json.load(file_to_test)

        with open(expected_output_file) as expected_file:
            expected_data = json.load(expected_file)

        for h in data_to_test:
            data_to_test[h].sort()

        for h in expected_data:
            expected_data[h].sort()

        self.assertDictEqual(data_to_test, expected_data, "Error: the content of the file created by   \
                                                           the fucntion - courses_for_lecturers        \
                                                           doesn't match the expected one.")

    def test4(self):
        """
        this test for the first function - names_of_registered_students.

        """

        correct_answer =  ['FFhaFFeed', 'Google', 'Harry Potter', 'Yazeed Falah', 'Zoabi']

        result = names_of_registered_students(
            input_json_path2, "AmirSchoolOfWitchcraftAndWizardry")

        correct_answer.sort()
        result.sort()

        self.assertListEqual(correct_answer, result, "Error: the output of the function   \
                                                  names_of_registered_students        \
                                                  doesn't match the expected one.")

    def test5(self):
        """
        this test for the second function - enrollment_number.
        """

        expected_output_file = "expected_output_test5_enrollment_numbers.txt"
        enrollment_numbers(input_json_path2, "test5.out")
        with open("test5.out") as file_to_test:
            data_to_test = list(file_to_test)

        with open(expected_output_file) as expected_file:
            expected_data = list(expected_file)

        self.assertListEqual(data_to_test, expected_data, "Error: the content of the file created by   \
                                              the fucntion - enrollment_number     \
                                              doesn't match the expected one.")

    def test6(self):
        """
        this test for the third function - courses_for_lecturers.
        """
        expected_output_file = "expected_output_test6_courses_for_lecturers.json"
        courses_for_lecturers(database_directory_path2, "test6_output.json")

        with open("test6_output.json") as file_to_test:
            data_to_test = json.load(file_to_test)

        with open(expected_output_file) as expected_file:
            expected_data = json.load(expected_file)

        for h in data_to_test:
            data_to_test[h].sort()

        for h in expected_data:
            expected_data[h].sort()

        self.assertDictEqual(data_to_test, expected_data, "Error: the content of the file created by   \
                                                           the fucntion - courses_for_lecturers        \
                                                           doesn't match the expected one.")

    def test7(self):
        """
        this test for the second function - enrollment_number.
        """

        expected_output_file = "expected_output_test7_enrollment_numbers.txt"
        enrollment_numbers(input_json_path2.replace("2", "3"), "test7.out")
        with open("test7.out") as file_to_test:
            data_to_test = list(file_to_test)

        with open(expected_output_file) as expected_file:
            expected_data = list(expected_file)

        self.assertListEqual(data_to_test, expected_data, "Error: the content of the file created by   \
                                              the fucntion - enrollment_number     \
                                              doesn't match the expected one.")

if __name__ == '__main__':
    unittest.main()
