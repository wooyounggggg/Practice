const express = require('express');
const app = express();
const logger = require('morgan');
const cookieParser = require('cookie-parser');
const path = require('path');
const session = require('express-session');
const flash = require('connect-flash');
const indexRouter = require('./routes/index');
const usersRouter = require('./routes/users');

app.use(logger('dev'));
app.use(express.static(path.join(__dirname, 'public'))); //정적 파일용 미들웨어
app.use(express.json());
app.use(express.urlencoded({
    extended: false
}));
app.use(cookieParser('secret code')); //cookie parsing 미들웨어
app.use(session({
    resave: false,
    saveUninitialized: false,
    secret: 'secret code',
    cookie: {
        httpOnly: true,
        secure: false,
    },
}));
app.use(flash()); //middle ware 순서 중요!!

app.use('/', indexRouter);
app.use('/users', usersRouter);

//404 error : 400번대 error -> client단 에러!
app.use((req, res, next) => {
    res.status(404).send('Not Found!');
});

//500 error : 500번대 error -> server단 에러!
app.use((err, req, res) => {
    console.log(err);
    res.status(500).send('SERVER ERROR');
});
module.exports = app;