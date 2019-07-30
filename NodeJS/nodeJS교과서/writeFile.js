const fs = require('fs');
const fileContents = 'Write here anything!'
fs.writeFile('./writeme.txt', fileContents, (err) => {
    if(err){
        throw err;
    }
});

fs.readFile('./writeme.txt',(err,data)=>{
    if(err) throw err;
    console.log(data.toString());
})