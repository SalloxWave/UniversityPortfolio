/* ================================== */
/* ==      Constant variables      == */
/* ================================== */
BASEURL = "http://localhost:3826/";
LOGGEDINCOOKIE = "loggedIn=\"true\"";
MONGO_URL = "localhost";
MONGO_PORT = 27017;
DB_NAME = "website3826";

/* =============================== */
/* ==      Library imports      == */
/* =============================== */
var request = require("superagent");  //https://www.npmjs.com/package/superagent
var assert = require("assert");  //https://www.npmjs.com/package/assert
var mongo = require('mongodb');
var mongoServer = new mongo.Server(MONGO_URL, MONGO_PORT);
var db = new mongo.Db(DB_NAME, mongoServer);

/* =================================== */
/* ==      Test help functions      == */
/* =================================== */
function removeAsFriends(callback) {
    var removeFriendData = {
        userName: "User1",
        friendName: "User2"
    };

    //Remove as friends before each test
    request.post(BASEURL + "api/removefriend")
    .set("Cookie", LOGGEDINCOOKIE)
    .send(removeFriendData).end(function(err, response) {                
        callback(err, response);
    });
}

function addFriends(callback) {
    var addFriendData = {
        userName: "User1",
        friendName: "User2"
    };
    request.post(BASEURL + "api/addfriend")
    .set("Cookie", LOGGEDINCOOKIE)
    .send(addFriendData).end(function(err, response) {
        callback(err, response);
    });
}

function clearPosts(callback) {
    var url = BASEURL + "api/clearposts";
    //Clear posts for both users
    request.post(url)
    .set("Cookie", LOGGEDINCOOKIE)
    .send({userName: "User1"}).end(function(err, response) {
        request.post(url)
        .set("Cookie", LOGGEDINCOOKIE)
        .send({userName: "User2"}).end(function(err, response) {
            callback(err, response);
        });
    });
}

function postToUser2(callback) {
    var postData = {
        ownerName: "User1",
        friendName: "User2",
        message: "User1's message"
    };
    request.post(BASEURL + "api/post")
    .set("Cookie", LOGGEDINCOOKIE)
    .send(postData).end(function(err, response) {
        callback(err, response);
    });
}

/* ============================== */
/* ==      Configure test      == */
/* ============================== */
//Runs on startup if "--delay"-flag is used
process.nextTick(function() {
    db.open(function (error, db) {
        //Clear database  
        db.dropDatabase();

        var credentials = {
            userName: "User1",
            email: "User1Email",
            password: "User1Password"
        };    
        var credentials2 = {
            userName: "User2",
            email: "User2Email",
            password: "User2Password"
        };

        //Register two users to use in tests
        request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {            
            request.post(BASEURL + "api/register").send(credentials2).end(function(err, response) {                
                run();  //Only exists when running with "--delay" flag
            });
        });            
    });
});

/* =========================== */
/* ==      Server test      == */
/* =========================== */
describe("Server functions for website3826", function() {
    describe("GET /", function() {
        it("No crashes (200)", function(done) {
            request.get(BASEURL, function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });            
        });
    });

    describe("GET /register", function() {
        it("No crashes (200)", function(done) {
            request.get(BASEURL + "register", function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });            
        });
    });

    describe("GET /login", function() {
        it("No crashes (200)", function(done) {
            request.get(BASEURL + "login", function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });            
        });
    });

    describe("GET /search", function() {
        it("No crashes (200)", function(done) {
            request.get(BASEURL + "search", function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });            
        });
    });

    describe("POST /api/register", function() {
        before(function() {
            url = BASEURL + "api/register";
        });

        it("No crashes (200)", function(done) {
            var credentials = {
                userName: "TestRegisterUserName",
                email: "TestRegisterEmail",
                password: "TestRegisterPassword"
            };            
            request.post(url).send(credentials).end(function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });
        });

        it("Register with åäö (200)", function(done) {
            var credentials = {
                userName: "åäö",
                email: "åäö",
                password: "åäö"
            };            
            request.post(url).send(credentials).end(function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });
        });

        it("Register when username already exists (409)", function(done) {
            var credentials = {
                userName: "User1",
                email: "ValidEmail",
                password: "Validpassword"
            };
            request.post(url).send(credentials).end(function(err, response) {
                assert.equal(response.statusCode, 409);
                done();
            });
        });

        it("Register when email already exists (409)", function(done) {
            var credentials = {
                userName: "ValidUserName",
                email: "User1Email",
                password: "Validpassword"
            };
            request.post(url).send(credentials).end(function(err, response) {
                assert.equal(response.statusCode, 409);
                done();
            });
        });
    });

    describe("GET /<username>", function() {
        it("User does not exists (404)", function(done) {
            request.get(BASEURL + "NonExistingUser", function(err, response) {
                assert.equal(response.statusCode, 404);
                done();
            });
        });

        it("User does exist (200)", function(done) {                        
            request.get(BASEURL + "User1", function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });
        });
    });

    describe("GET /api/<username>", function() {
        it("User does not exist (400)", function(done) {
            request.get(BASEURL + "api/NonExistingUser", function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("User does exist", function(done) {            
            request.get(BASEURL + "api/User1", function(err, response) {
                assert.equal(response.statusCode, 200);
                assert.equal(response.body.userName, "User1");
                assert.equal(response.body.email, "User1Email");
                assert.notEqual(response.body.password, "User1Password");
                done();
            });             
        });
    });

    describe("POST /api/takenname", function() {   
        before(function() {
            url = BASEURL + "api/takenname";
        });

        it("Name not taken (false)", function(done) {
            var notTakenName = {userName: "NotTakenName"};
            request.post(url).send(notTakenName).end(function(err, response) {
                assert.equal(response.text, "false");
                done();
            });
        });

        it("Name taken (true)", function(done) {        
            request.post(url).send({userName: "User1"}).end(function(err, response) {
                assert.equal(response.text, "true");
                done();
            });         
        });
    });

    describe("POST /api/takenemail", function() { 
        before(function() {
            url = BASEURL + "api/takenemail";
        });

        it("Email not taken (200)", function(done) {
            var notTakenName = {userName: "NotTakenEmail"};
            request.post(url).send(notTakenName).end(function(err, response) {
                assert.equal(response.statusCode, 200);
                assert.equal(response.text, "false");
                done();
            });
        });

        it("Email taken (200)", function(done) {        
            request.post(url).send({email: "User1Email"}).end(function(err, response) {
                assert.equal(response.statusCode, 200);
                assert.equal(response.text, "true");
                done();
            });
        });
    });

    describe("POST /api/login", function() {
        before(function() {
            url = BASEURL + "api/login";
        });

        it("Login with invalid login name (401)", function(done) {
            var loginData = {
                loginName: "InvalidLoginName",
                password: "DoesntMatter"
            };
            request.post(url).send(loginData).end(function(err, response) {
                assert.equal(response.statusCode, 401);
                done();
            });
        });

        it("Login with invalid password (401)", function(done) {            
            var loginData = {
                loginName: "User1",
                password: "InvalidPassword"
            };            
            request.post(url).send(loginData).end(function(err, response) {
                assert.equal(response.statusCode, 401);
                done();
            });
        });

        it("Login with username (200)", function(done) {            
            var loginData = {
                loginName: "User1",
                password: "User1Password"
            };            
            request.post(url).send(loginData).end(function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });
        });

        it("Login with email (200)", function(done) {
            var loginData = {
                loginName: "User1Email",
                password: "User1Password"
            };       
            request.post(url).send(loginData).end(function(err, response) {
                assert.equal(response.statusCode, 200);
                done();
            });
        });
    });

    //Remove since result is now rendered instead of sent in body
    // describe("GET /search", function() {
    //     it("Search for non-existing user (200)", function(done) {
    //         var url = BASEURL + "search?q=DontSearchMePls";
    //         request.get(url, function(err, response) {
    //             assert.equal(response.statusCode, 200);
    //             assert.equal(response.body.length, 0);
    //             done();
    //         });
    //     });

    //     it("Search for existing user name (200)", function(done) {
    //         var credentials = {
    //             userName: "ExistingUsername",
    //             email: "SearchMeEmail",
    //             password: "SearchMePassword"
    //         };
    //         request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {
    //             var url = BASEURL + "search?q=ExistingUsername";
    //             request.get(url, function(err, response) {
    //                 assert.equal(response.statusCode, 200);
    //                 assert.equal(response.body.length, 1);
    //                 assert.equal(response.body[0].userName, "ExistingUsername");
    //                 done();
    //             });
    //         });
    //     });

    //     it("Search for existing email (200)", function(done) {
    //         var credentials = {
    //             userName: "SearchMeEmail",
    //             email: "ExistingEmail",
    //             password: "SearchMePassword"
    //         };
    //         request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {
    //             var url = BASEURL + "search?q=ExistingEmail";
    //             request.get(url, function(err, response) {
    //                 assert.equal(response.statusCode, 200);
    //                 assert.equal(response.body.length, 1);
    //                 assert.equal(response.body[0].userName, "SearchMeEmail");
    //                 done();
    //             });
    //         });
    //     });

    //     it("Search for multiple usernames (200)", function(done) {
    //         var credentials = {
    //             userName: "MultipleUserName",
    //             email: "MultipleUserNameEmail",
    //             password: "MultipleUserNamePassword"
    //         };
    //         request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {
    //             credentials.userName+="2";
    //             credentials.email+="2";
    //             request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {                                        
    //                 credentials.userName+="3";
    //                 credentials.email+="3";
    //                 request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {                        
    //                     var url = BASEURL + "search?q=MultipleUserName";
    //                     request.get(url, function(err, response) {
    //                         assert.equal(response.statusCode, 200);
    //                         assert.equal(response.body.length, 3);
    //                         assert.equal(response.body[0].userName, "MultipleUserName");
    //                         assert.equal(response.body[1].userName, "MultipleUserName2");
    //                         assert.equal(response.body[2].userName, "MultipleUserName23");
    //                         done();
    //                     });
    //                 });
    //             });
    //         });
    //     });

    //     it("Search for usernames with limit (200)", function(done) {
    //         var credentials = {
    //             userName: "LimitUserName",
    //             email: "LimitEmail",
    //             password: "LimitPassword"
    //         };
    //         request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {
    //             credentials.userName+="2";
    //             credentials.email+="2";
    //             request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {                                        
    //                 credentials.userName+="3";
    //                 credentials.email+="3";
    //                 request.post(BASEURL + "api/register").send(credentials).end(function(err, response) {                        
    //                     var url = BASEURL + "search?q=LimitUserName&l=2";
    //                     request.get(url, function(err, response) {
    //                         assert.equal(response.statusCode, 200);
    //                         assert.equal(response.body.length, 2);
    //                         assert.equal(response.body[0].userName, "LimitUserName");
    //                         assert.equal(response.body[1].userName, "LimitUserName2");
    //                         done();
    //                     });
    //                 });
    //             });
    //         });
    //     });
    // });

    describe("POST /api/friendrequest", function() {
        before(function() {
            url = BASEURL + "api/friendrequest";
        });

        it("Send friend request without being logged in (400)", function(done) {
            request.post(url, function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Send friend request to non-existing friend (400)", function(done) {
            var friendRequestData = {
                userName: "User1",
                friendName: "NonExistingFriend"
            };            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(friendRequestData).end(function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Send friend request to yourself (400)", function(done) {                      
            var friendRequestData = {
                userName: "User1",
                friendName: "User1"
            };            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(friendRequestData).end(function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Send friend request working (200)", function(done) {
            var friendRequestData = {
                userName: "User1",
                friendName: "User2"
            };            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(friendRequestData).end(function(err, response) {
                assert.equal(response.statusCode, 200);

                //User2 should have received the friend request
                request.get(BASEURL + "api/User2", function(err, response) {
                    assert.equal(response.body.friendRequests.length, 1);
                    assert.equal(response.body.friendRequests[0], "User1");                            
                    done();
                });
            });            
        });

        it("Send friend request when already sent (400)", function(done) {
            var friendRequestData = {
                userName: "User1",
                friendName: "User2"
            };
            //Send double friend request            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(friendRequestData).end(function() {
                request.post(url)
                .set("Cookie", LOGGEDINCOOKIE)
                .send(friendRequestData).end(function(err, response) {
                    assert.equal(response.statusCode, 400);

                    //User2 will still have only one friend request
                    request.get(BASEURL + "api/User2", function(err, response) {
                        assert.equal(response.body.friendRequests.length, 1);
                        assert.equal(response.body.friendRequests[0], "User1");                            
                        done();
                    });
                });
            });       
        });

        it("Send friend request when already friends (400)", function(done) {
            addFriends(function() {
                var friendRequestData = {
                    userName: "User1",
                    friendName: "User2"
                };

                //Send request when already friends
                request.post(url)
                .set("Cookie", LOGGEDINCOOKIE)
                .send(friendRequestData).end(function(err, response) {
                    assert.equal(response.statusCode, 400)
                    done();
                });
            });
        });
    });

    describe("POST /api/addfriend", function() {
        before(function() {
            url = BASEURL + "api/addfriend";
        });

        beforeEach(function(done) {
            //Reset friendship after each test         
            removeAsFriends(function() {
                done();
            });
        });

        it("Add friend without being logged in (400)", function(done) {
            request.post(url, function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });           
        });

        it("Add non-existing friend (400)", function(done) {            
            var addFriendData = {
                userName: "User1",
                friendName: "ImaginaryFriend"
            };            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(addFriendData).end(function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Add yourself as friend (400)", function(done) {            
            var addFriendData = {
                userName: "User1",
                friendName: "User1"
            };            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(addFriendData).end(function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Add friend working (200)", function(done) {            
            var addFriendData = {
                userName: "User1",
                friendName: "User2"
            };            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(addFriendData).end(function(err, response) {
                assert.equal(response.statusCode, 200);

                //Make sure friends have updated for both
                request.get(BASEURL + "api/User1", function(err, response) {
                    assert.equal(response.body.friends[0], "User2");                            
                    assert.equal(response.body.friendRequests.length, 0);

                    request.get(BASEURL + "api/User2", function(err, response) {
                        assert.equal(response.body.friends[0], "User1");
                        assert.equal(response.body.friendRequests.length, 0);
                        done();
                    });
                });                        
            });           
        });

        it("Add as friend when already friends (400)", function(done) {
            var addFriendData = {
                userName: "User1",
                friendName: "User2"
            };            
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(addFriendData).end(function() {
                request.post(url)
                .set("Cookie", LOGGEDINCOOKIE)
                .send(addFriendData).end(function(err, response) {
                    assert.equal(response.statusCode, 400);
                    done();
                });
            });
        });
    });

    describe("POST /api/removefriend", function() {
        before(function() {
            url = BASEURL + "api/removefriend";
        });

        it("Remove friend without being logged in (400)", function(done) {
            request.post(url, function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Remove friend without being friends (400)", function(done) {
            var removeFriendData = {
                userName: "User1",
                friendName: "IDontWantToBeAnyonesFriend"
            };
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE).end(function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Remove friend working (200)", function(done) {
            addFriends(function() {
                var removeFriendData = {
                    userName: "User1",
                    friendName: "User2"
                };

                request.post(url)
                .set("Cookie", LOGGEDINCOOKIE)
                .send(removeFriendData).end(function(err, response) {
                    assert.equal(response.statusCode, 200);

                    //Make sure friends have been removed for both users
                    request.get(BASEURL + "api/User1", function(err, response) {
                        assert.equal(response.body.friends.length, 0);
    
                        request.get(BASEURL + "api/User2", function(err, response) {
                            assert.equal(response.body.friends.length, 0);                            
                            done();
                        });
                    });                    
                });
            });          
        });
    });

    describe("POST /api/post", function() {
        before(function() {
            url = BASEURL + "api/post";
        });

        beforeEach(function(done) {
            //Reset friendship and posts before each test
            removeAsFriends(function() {
                clearPosts(function() {
                    done();
                });
            });
        });

        it("Post without being logged in (400)", function(done) {
            request.post(url, function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Post without being friends (400)", function(done) {
            var postData = {
                userName: "User1",
                friendName: "IDontWantToBeAnyonesFriend"
            };
            request.post(url)
            .set("Cookie", LOGGEDINCOOKIE)
            .send(postData).end(function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Post without crashes (200)", function(done) {
            addFriends(function() {
                var postData = {
                    ownerName: "User1",
                    friendName: "User2",
                    message: "User1's message"
                };            
                request.post(url)
                .set("Cookie", LOGGEDINCOOKIE)
                .send(postData).end(function(err, response) {
                    assert.equal(response.statusCode, 200);
                    
                    //Make sure post has been posted
                    request.get(BASEURL + "api/User2/posts", function(err, response) {
                        assert.equal(response.body.length, 1);
                        assert.equal(response.body[0].ownerName, "User1");
                        assert.equal(response.body[0].message, "User1's message");
                        done();
                    });
                });
            });
        });
    });

    describe("GET /api/<username>/posts", function() {
        beforeEach(function(done) {
            //Reset friendship and posts before each test
            removeAsFriends(function() {
                clearPosts(function() {
                    done();
                });
            });
        });

        it("Non-existing user (400)", function(done) {
            var url = BASEURL + "api/IDontExist/posts";
            request.get(url, function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Get posts without crashes (200)", function(done) {
            addFriends(function() {
                postToUser2(function() {
                    request.get(BASEURL + "api/User2/posts", function(err, response) {
                        assert.equal(response.statusCode, 200);
                        assert.equal(response.body.length, 1);
                        assert.equal(response.body[0].ownerName, "User1");
                        assert.equal(response.body[0].message, "User1's message");
                        done();
                    });
                });                
            });
        });
    });

    describe("POST /api/clearposts", function() {
        before(function() {
            url = BASEURL + "api/clearposts";
        });

        beforeEach(function(done) {
            //Reset friendship and posts before each test
            removeAsFriends(function() {
                clearPosts(function() {
                    done();
                }); 
            });        
        });

        it("Clear posts without being logged in (400)", function(done) {
            request.post(url, function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Clear two posts", function(done) {
            addFriends(function() {
                postToUser2(function() {
                    postToUser2(function() {

                        //Clear posts on user2's profile
                        request.post(url)
                        .set("Cookie", LOGGEDINCOOKIE)
                        .send({userName: "User2"}).end(function(err, response) {
                            assert.equal(response.statusCode, 200);

                            //Make sure both posts have been removed
                            request.get(BASEURL + "api/User2/posts", function(err, response) {                                
                                assert.equal(response.body.length, 0);                                
                                done();
                            });
                        });
                    });
                });                
            });
        });
    });

    describe("POST /api/clearposted", function() {
        before(function() {
            url = BASEURL + "api/clearposted";
        });

        beforeEach(function(done) {
            //Reset friendship and posts before each test
            removeAsFriends(function() {
                clearPosts(function() {
                    done();
                }); 
            });        
        });

        it("Clear posted posts without being logged in (400)", function(done) {
            request.post(url, function(err, response) {
                assert.equal(response.statusCode, 400);
                done();
            });
        });

        it("Clear two posted posts", function(done) {
            addFriends(function() {
                postToUser2(function() {
                    postToUser2(function() {

                        //Clear posts on user2's profile
                        request.post(url)
                        .set("Cookie", LOGGEDINCOOKIE)
                        .send({userName: "User1"}).end(function(err, response) {
                            assert.equal(response.statusCode, 200);

                            //Make sure both posts have been removed
                            request.get(BASEURL + "api/User2/posts", function(err, response) {                                
                                assert.equal(response.body.length, 0);                                
                                done();
                            });
                        });
                    });
                });                
            });
        });
    });

    describe("GET /api/<username>/posted", function() {
        beforeEach(function(done) {
            //Reset friendship and posts before each test
            removeAsFriends(function() {
                clearPosts(function() {
                    done();
                });
            });            
        });

        it("Non-existing user (404)", function(done) {            
            request.get(BASEURL + "api/IDontExist/posted", function(err, response) {
                assert.equal(response.statusCode, 404);
                done();
            });
        });

        it("Get posted without crashes (200)", function(done) {
            addFriends(function() {
                postToUser2(function() {
                    request.get(BASEURL + "api/User1/posted", function(err, response) {
                        assert.equal(response.statusCode, 200);
                        assert.equal(response.body.length, 1);
                        assert.equal(response.body[0].ownerName, "User1");
                        assert.equal(response.body[0].message, "User1's message");
                        done();
                    });
                });
            });
        });
    });

    after(function() {        
        //Make sure to clear database entries and closing database before quitting tests
        db.dropDatabase();
        db.close();
    });
});