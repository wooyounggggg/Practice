const fs = require('fs');
fs.readFile('./readme.txt',(err,data)=>{
    if(err){
        throw err;
    }
    console.log(data); //buffer read
    console.log(data.toString()); //txt read
});