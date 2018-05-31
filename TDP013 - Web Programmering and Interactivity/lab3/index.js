MONGO_PORT = 27017;
MONGO_SERVER = "localhost";
MONGO_DB_NAME = "tdp013";

var express = require('express');
var app = express();
var mongo = require('mongodb');

var server = new mongo.Server(MONGO_SERVER, MONGO_PORT);
var db = new mongo.Db(MONGO_DB_NAME, server);

//Open the database connection
db.open(function (err, db) {
    if(!err) {
        console.log("Connected to db " + MONGO_DB_NAME + " on port " + MONGO_PORT);
        console.log("Server located at " + MONGO_SERVER);        
    }
    else {
        console.log("Error occured when trying to connect to database: " + err.message);
    }
});

app.use(function(req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    next();
});

app.get('/', function (request, response) {
    response.sendFile('dummy_index.html', {root: __dirname});
});

// If someone requests the /save path and wants to save a new tweet to the database
app.get('/save', function (request, response) {
    
    //Get message from url parameter    
    var msg = request.query.m;

    if (msg == null || msg.length > 140 || msg.length == 0) {
        response.status(400).end();
        return;
    }    
    
    //Access the database collection with tweets
    db.collection('tweets', function (err, collection) {
        if(!err) {
            //Create unread tweet with specific message
            var tweet = {
                message:msg,
                flag:false //If msg is read or not
            }

            //Insert tweet object in database
            collection.insert(tweet, function (err, result) {
                if(!err) {
                    console.log("Successully added new tweet to database: " +  tweet._id);                                        
                    response.send(tweet);
                    response.status(200).end();
                }
                else {
                    response.status(500).end();                    
                }
            });
        } 
        else {
            response.status(500).end();
        }
    });
});

//Call to mark a specified post as read
app.get('/flag', function (request, response) {
    
    //Get message from url parameter
    var id = request.query.ID;

    //Missing parameter or empty id
    if (!id) {
         response.status(400).end();
         return;
    }
    
    //Access the database collection with tweets
    db.collection('tweets', function (err, collection) {
        if(!err) {
            //Create search query to find object to update
            var query = {"_id": new mongo.ObjectId(id)};
            
            //Update flag of object with specified id
            collection.update(query, {$set:{flag: true}}, function (err, result) {
                if(!err) {
                    console.log("Sucessfully marked post " + id + " as read (" + result + ")");
                    response.status(200).end();
                }
                else {
                    response.status(500).end();
                    return;
                }
            });
            
        }
        else {
            response.status(500).end();
            return;
        }
    });
});

app.get('/getall', function (request, response) {

    //Access the database collection with tweets
    db.collection('tweets', function (err, collection) {
        if (!err) {
            //Search to match all entries in database            
            collection.find({}).toArray(function (err, result) {
                if (!err) {
                    response.send(result);
                    console.log("Sent all tweets from database");
                    response.status(200).end();
                }
                else {
                    response.status(500).end();
                    return;
                }
            });
        }
        else {
            response.status(500).end();
            return;
        }
    });
});

app.get('*', function (request, response) {
    response.status(404).end();
});

app.all('*', function (request, response) {
    response.status(405).end();
});

app.listen(3826);