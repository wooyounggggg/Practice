const path = require('path');
const Sequelize = require('sequelize');

const env = process.env.NODE_ENV || 'development'; //production
const config = require('../config/config.json')[env];

const sequelize = new Sequelize(config.database, config.username, config.password, config);

const db = {};

db.Sequelize = sequelize;
db.sequelize = sequelize;

module.exports = db;