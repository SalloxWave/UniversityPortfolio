# How to run server
1. Make sure to run command "npm install" if first time.
2. Start mongo database: "sudo service mongod start".
3. Run command "npm start".
4. Open browser and visist address: "http://localhost:3826/".

# How to run tests
## Mocha test
1. Make sure server is running
2. Run command "npm test"

## Mocha test with istanbul
This does not work as intended right now due to the required "--delay" 
doesn't work as intended together with istanbul. "--delay" flag is used to be able to 
run code before starting the tests, for example register users. I will fix this if there's time.
1. Make sure server is running.
2. Run command "npm test_istanbul".

## Selenium test
1. Make sure database doesn't contain any conflict with test
2. Make sure server is running.
3. Run command "npm run selenium".
4. Specific tests:
- npm run selenium_nav: Run navigation test
- npm run selenium_account: Create account, login and search for user
- npm run selenium_friend_post: Add and remove friend, post on friend's profile