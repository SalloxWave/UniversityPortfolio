from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.support.ui import WebDriverWait
import unittest
import time

address = "http://localhost:8000"

# Create a new instance of the Firefox driver and load page
driver = webdriver.Firefox()
driver.get(address)

class AlmostTwitterTest(unittest.TestCase):

    def setUp(self):
        self.driver = webdriver.Firefox()

    def post(self, message):
        inputElement = self.driver.find_element_by_id("text")
        inputElement.send_keys(message)
        inputElement.submit()

    def test_post(self):
        driver = self.driver
        driver.get(address)

        message = "Cheese is the most awesome thing on earth"
        self.post(message)

        count = len(driver.find_elements_by_xpath("//div[@id='postList']/li"))
        assert count == 1
        print("Added post :D")

        text = driver.find_element_by_xpath("//li[@id='post0']/span").get_attribute("innerHTML")
        assert text == message
        print("Message is right :D")
    
    def test_post_limit(self):
        driver = self.driver
        driver.get(address)

        message = "140 characters140 characters140 characters140 characters140 characters140 characters140 characters140 characters140 characters140 characters TOoMANY"
        self.post(message)

        count = len(driver.find_elements_by_xpath("//div[@id='postList']/li"))
        assert count == 0
        print("Char Limit :D")

        errorVisable = driver.find_element_by_id("postError").value_of_css_property("display")
        assert errorVisable == "block"
        print("Error Shows :D")

    def test_empty_post(self):
        driver = self.driver
        driver.get(address)

        message = ""
        self.post(message)

        count = len(driver.find_elements_by_xpath("//div[@id='postList']/li"))
        assert count == 0
        print("Empty post is success :D")

        errorVisable = driver.find_element_by_id("postError").value_of_css_property("display")
        assert errorVisable == "block"
        print("Error Shows :D")

    def test_chronologic_order(self):
        driver = self.driver
        driver.get(address)

        message = "First Message"
        self.post(message)

        message = "Second Message"
        self.post(message)

        # Testing the newest post
        newest_post = driver.find_elements_by_xpath("//div[@id='postList']/li")[0]
        assert newest_post.get_attribute("id") == "post1"
        assert newest_post.find_element_by_tag_name("span").get_attribute("innerHTML") == "Second Message"
        print("Newest Post :D")

        # Testing the oldest post
        oldest_post = driver.find_elements_by_xpath("//div[@id='postList']/li")[1]
        assert oldest_post.get_attribute("id") == "post0"
        assert oldest_post.find_element_by_tag_name("span").get_attribute("innerHTML") == "First Message"
        print("Oldest Post :D")

    def test_mark_as_read(self):
        driver = self.driver
        driver.get(address)

        message = "This is a new awesome post. You should read this."
        self.post(message)

        # Testing the mark as read button
        post = driver.find_elements_by_xpath("//div[@id='postList']/li")[0]
        assert post.get_attribute("class") == ""
        post.find_element_by_tag_name("button").click()
        assert post.get_attribute("class") == "read"
        print("Post just read :D")

    def test_post_reload(self):
        driver = self.driver
        driver.get(address)
        
        message = "This is a new awesome post. You should read this!"
        self.post(message)
        assert len(driver.find_elements_by_xpath("//div[@id='postList']/li")) == 1

        driver.refresh()
        assert len(driver.find_elements_by_xpath("//div[@id='postList']/li")) == 0

    def test_selenium(self):
        assert driver
        print("Yes, selenium exist")

    def tearDown(self):
        #self.driver.close()
        self.driver.quit()



if __name__ == "__main__":
    unittest.main()