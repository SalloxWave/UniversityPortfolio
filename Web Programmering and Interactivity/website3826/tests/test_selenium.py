# Note: XPath copied from browser's inspect mode

from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
import unittest
import time

#Disable alphabetical executing order (doesn't work)
unittest.TestLoader.sortTestMethodsUsing = None

BASEURL = "http://localhost:3826/"

#Global driver that should be used everywhere
driver = None

def register(userName, email, password):    
    global driver
    driver.get(BASEURL + "register")
    #Fill register form
    elUserName = driver.find_element_by_id("userName")
    elUserName.send_keys(userName)    
    elEmail = driver.find_element_by_id("email")
    elEmail.send_keys(email)    
    elPassword = driver.find_element_by_id("password")
    elPassword.send_keys(password)    

    #Submit form
    elUserName.submit()

    try:
        _ = WebDriverWait(driver, 10).until(
        EC.title_contains("Registered"))
    except TimeoutException:
        pass

def login(userName, password):
    global driver
    driver.get(BASEURL + "login")
    #Fill login form
    elLoginName = driver.find_element_by_id("loginName")
    elLoginName.send_keys(userName)    
    elPassword = driver.find_element_by_id("password")
    elPassword.send_keys(password)    

    #Submit form
    elLoginName.submit()
    try:
        _ = WebDriverWait(driver, 10).until(
        EC.title_contains(userName))
    except TimeoutException:
        pass

def logout():
    global driver
    driver.get(BASEURL)
    logout = driver.find_element_by_xpath("/html/body/nav/div/ul/li[2]/a")
    logout.click()

class A_NavigationTest(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        global driver
        #Open new browser before running tests
        driver = webdriver.Firefox()

    def setUp(self):
        global driver
        self.driver = driver

    def test_nav_home(self):
        self.driver.get(BASEURL + "search")        
        home = self.driver.find_element_by_xpath("/html/body/nav/div/a[1]")
        home.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Home"))
        except TimeoutException:
            self.fail()

    def test_nav_search(self):
        self.driver.get(BASEURL)
        search = self.driver.find_element_by_xpath("/html/body/nav/div/a[2]")
        search.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Search"))
        except TimeoutException:
            self.fail()        

    def test_nav_register(self):
        self.driver.get(BASEURL)        
        register = self.driver.find_element_by_xpath("/html/body/nav/div/ul/li[1]/a")
        register.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Register"))
        except TimeoutException:
            self.fail()        

    def test_nav_login(self):
        self.driver.get(BASEURL)        
        login = self.driver.find_element_by_xpath("/html/body/nav/div/ul/li[2]/a")
        login.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Login"))
        except TimeoutException:
            self.fail()        

    def test_nav_home_register(self):
        self.driver.get(BASEURL)        
        register = self.driver.find_element_by_xpath("/html/body/div[1]/div/div/p[2]/a")
        register.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Register"))
        except TimeoutException:
            self.fail()
    
    def test_nav_home_register(self):
        self.driver.get(BASEURL)        
        login = self.driver.find_element_by_xpath("/html/body/div[1]/div/div/p[3]/a")
        login.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Login")
        )
        except TimeoutException:
            self.fail()
    
    def test_nav_register_login(self):
        self.driver.get(BASEURL + "register")        
        login = self.driver.find_element_by_xpath("/html/body/div/div/div/p[2]/a")
        login.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Login"))
        except TimeoutException:
            self.fail()

    def test_nav_login_register(self):
        self.driver.get(BASEURL + "login")
        register = self.driver.find_element_by_xpath("/html/body/div/div/div/p[2]/a")
        register.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Register"))
        except TimeoutException:
            self.fail()

    def test_nav_registered_login(self):
        self.driver.get(BASEURL + "registered")
        login = self.driver.find_element_by_xpath("/html/body/div/div/div/p[2]/a")
        login.click()
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Login"))
        except TimeoutException:
            self.fail()

    @classmethod
    def tearDownClass(cls):
        global driver
        driver.delete_all_cookies()            
        driver.close()        

class B_CreateAccount(unittest.TestCase):

    def get_search_results(self):
        return self.driver.find_elements_by_xpath("/html/body/div[2]/div/div[2]/ul/*")
    
    @classmethod
    def setUpClass(cls):
        global driver
        #Open new browser before running tests
        driver = webdriver.Firefox()

    def setUp(self):
        global driver
        self.driver = driver
        self.userName = "Username"
        self.email = "User@email.com"
        self.password = "UserPassword"

        self.userNameNotTaken = "Username2"
        self.emailNotTaken = "User2@email.com"        

    def test_a_search_empty(self):
        self.driver.get(BASEURL + "search")
        search = self.driver.find_element_by_id("btnSearch")
        search.click()
        
        assert len(self.get_search_results()) == 0

    def test_b_register(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys(self.userName)
        email = self.driver.find_element_by_id("email")
        email.send_keys(self.email)
        password = self.driver.find_element_by_id("password")
        password.send_keys(self.password)

        #Submit form
        userName.submit()

        #You should come to registered page
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Registered"))
        except TimeoutException:
            self.fail()

    def test_c_register_empty(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys("")
        email = self.driver.find_element_by_id("email")
        email.send_keys("")
        password = self.driver.find_element_by_id("password")
        password.send_keys("")

        #Submit form
        userName.submit()
        
        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("errorUsername")        
        assert "between" in err.get_attribute("innerHTML")
        
        err = self.driver.find_element_by_id("errorPassword")
        assert "between" in err.get_attribute("innerHTML")

    def test_d_register_username_too_short(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys("1234")
        email = self.driver.find_element_by_id("email")
        email.send_keys(self.emailNotTaken)
        password = self.driver.find_element_by_id("password")
        password.send_keys(self.password)

        #Submit form
        userName.submit()
        
        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("errorUsername")        
        assert "between" in err.get_attribute("innerHTML")

    def test_e_register_password_too_short(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys(self.userNameNotTaken)
        email = self.driver.find_element_by_id("email")
        email.send_keys(self.emailNotTaken)
        password = self.driver.find_element_by_id("password")
        password.send_keys("1234")

        #Submit form
        userName.submit()
        
        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("errorPassword")
        assert "between" in err.get_attribute("innerHTML")

    def test_f_register_username_too_long(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys("1234567890123456") #16 characters
        email = self.driver.find_element_by_id("email")
        email.send_keys(self.emailNotTaken)
        password = self.driver.find_element_by_id("password")
        password.send_keys(self.password)

        #Submit form
        userName.submit()
        
        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("errorUsername")        
        assert "between" in err.get_attribute("innerHTML")

    def test_g_register_password_too_long(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys("")
        email = self.driver.find_element_by_id("email")
        email.send_keys("")
        password = self.driver.find_element_by_id("password")
        password.send_keys("1234567890123456") #16 characters

        #Submit form
        userName.submit()
        
        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("errorPassword")
        assert "between" in err.get_attribute("innerHTML")

    def test_h_register_username_taken(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys(self.userName)
        email = self.driver.find_element_by_id("email")
        email.send_keys(self.emailNotTaken)
        password = self.driver.find_element_by_id("password")
        password.send_keys(self.password)

        #Submit form
        userName.submit()
        
        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("errorUsername")
        assert "taken" in err.get_attribute("innerHTML")

    def test_i_register_email_taken(self):
        self.driver.get(BASEURL + "register")
        #Fill register form
        userName = self.driver.find_element_by_id("userName")
        userName.send_keys(self.userNameNotTaken)
        email = self.driver.find_element_by_id("email")
        email.send_keys(self.email)
        password = self.driver.find_element_by_id("password")
        password.send_keys(self.password)

        #Submit form
        userName.submit()
        
        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("errorEmail")
        assert "taken" in err.get_attribute("innerHTML")        

    def test_j_login_invalid_username(self):
        self.driver.get(BASEURL + "login")
        #Fill login form
        loginName = self.driver.find_element_by_id("loginName")
        loginName.send_keys("invalid")       
        password = self.driver.find_element_by_id("password")
        password.send_keys(self.password)

        #Submit form
        loginName.submit()

        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.text_to_be_present_in_element((By.ID, "errLogin"), "Invalid credentials"))
        except TimeoutException:
            self.fail()

    def test_k_login_invalid_password(self):
        self.driver.get(BASEURL + "login")
        #Fill login form
        loginName = self.driver.find_element_by_id("loginName")
        loginName.send_keys(self.userName)
        password = self.driver.find_element_by_id("password")
        password.send_keys("invalid")

        #Submit form
        loginName.submit()

        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.text_to_be_present_in_element((By.ID, "errLogin"), "Invalid credentials"))
        except TimeoutException:
            self.fail()        

    def test_l_login_username(self):
        self.driver.get(BASEURL + "login")
        #Fill login form
        loginName = self.driver.find_element_by_id("loginName")
        loginName.send_keys(self.userName)       
        password = self.driver.find_element_by_id("password")
        password.send_keys(self.password)

        #Submit form
        loginName.submit()

        #You should come to your own profile page
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains(self.userName))
        except TimeoutException:
            self.fail()

    def test_m_search_username(self):
        self.driver.get(BASEURL + "search")        
        query = self.driver.find_element_by_id("txtQuery")
        query.send_keys(self.userName)
        
        search = self.driver.find_element_by_id("btnSearch")
        search.click()

        assert len(self.get_search_results()) == 1

    def test_n_search_username(self):
        self.driver.get(BASEURL)
        logout = self.driver.find_element_by_xpath("/html/body/nav/div/ul/li[2]/a")
        logout.click()

        #You should come to home
        try:
            _ = WebDriverWait(self.driver, 10).until(
            EC.title_contains("Home"))
        except TimeoutException:
            self.fail()

        #Also, logout button should disappear
        logout = self.driver.find_element_by_xpath("/html/body/nav/div/ul/li[2]/a")
        assert logout.get_attribute("innerHTML") != "Logout"

    @classmethod
    def tearDownClass(cls):
        global driver
        driver.delete_all_cookies()            
        driver.close()

class C_FriendAndPost(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        global driver
        #Open new browser before running tests
        driver = webdriver.Firefox()

        #Register two initial users wanting to become friends :D
        register("Friend", "Friend@email.com", "password")        
        register("Friend2", "Friend2@email.com", "password")        

        #Login first user
        login("Friend", "password")

    def setUp(self):
        global driver
        self.driver = driver
        self.userName = "Friend"
        self.email = "Friend@email.com"
        self.password = "password"

        self.userName2 = "Friend2"
        self.email2 = "Friend2@email.com"
        self.password2 = "password"

    def test_a_post_not_friends(self):        
        self.driver.get(BASEURL + self.userName2)
        message = self.driver.find_element_by_id("txtMessage")
        message.send_keys("dummy message")
        btnPost = self.driver.find_element_by_id("btnPost")

        btnPost.click()

        self.driver.implicitly_wait(1)
        err = self.driver.find_element_by_id("postAlert")
        assert "Must be friends" in err.get_attribute("innerHTML")

    def test_b_send_friend_request(self):
        self.driver.get(BASEURL + self.userName2)
        btnRequest = self.driver.find_element_by_xpath("/html/body/div[1]/div/div/p[1]/button")
        btnRequest.click()

        self.driver.implicitly_wait(1)
        result = self.driver.find_element_by_id("friendAlert")
        assert "Successfully sent" in result.get_attribute("innerHTML")

    def test_c_friend_request_already_sent(self):
        self.driver.get(BASEURL + self.userName2)
        btnRequest = self.driver.find_element_by_xpath("/html/body/div[1]/div/div/p[1]/button")
        assert btnRequest.get_attribute("disabled") == "true"

    def test_d_accept_friend_request(self):
        logout()
        login(self.userName2, self.password2) #Automatic redirect to profile

        btnAccept = self.driver.find_element_by_id("btn" + self.userName)
        btnAccept.click()

        friends = self.driver.find_elements_by_xpath("//*[@id=\"friendList\"]/*")
        assert len(friends) == 1
        assert self.userName in friends[0].get_attribute("innerHTML")

        #Friends on other side as well
        self.driver.get(BASEURL + self.userName)
        friends = self.driver.find_elements_by_xpath("//*[@id=\"friendList\"]/*")
        assert len(friends) == 1
        assert self.userName2 in friends[0].get_attribute("innerHTML")

    def test_e_post_to_friend(self):
        self.driver.get(BASEURL + self.userName)
        message = self.driver.find_element_by_id("txtMessage")
        message.send_keys("Awesome test message")
        btnPost = self.driver.find_element_by_id("btnPost")

        btnPost.click()

        posts = self.driver.find_elements_by_xpath("//*[@id=\"postList\"]/*")
        msg = self.driver.find_element_by_xpath("//*[@id=\"postList\"]/li/h3").get_attribute("innerHTML")
        poster = self.driver.find_element_by_xpath("//*[@id=\"postList\"]/li/a").get_attribute("innerHTML")
        assert len(posts) == 1
        assert "Awesome test message" == msg
        assert self.userName2 in poster

    def test_f_remove_friend(self):
        self.driver.get(BASEURL + self.userName)
        btnRemove = self.driver.find_element_by_xpath("/html/body/div[1]/div/div/p[1]/button")
        btnRemove.click()

        self.driver.implicitly_wait(1)
        friends = self.driver.find_elements_by_xpath("//*[@id=\"friendList\"]/*")
        assert len(friends) == 0

        #Friend removed on both parts
        self.driver.get(BASEURL + self.userName2)
        friends = self.driver.find_elements_by_xpath("//*[@id=\"friendList\"]/*")
        assert len(friends) == 0

    @classmethod
    def tearDownClass(cls):
        global driver
        driver.delete_all_cookies()            
        driver.close()
        

if __name__ == "__main__":    
    unittest.main()