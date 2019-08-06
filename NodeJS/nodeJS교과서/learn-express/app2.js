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
module.exports = app;