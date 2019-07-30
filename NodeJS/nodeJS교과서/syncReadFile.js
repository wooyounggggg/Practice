const fs = require('fs');
const util = require('util');
const readFilePromise = util.promisify(fs.readFile);

console.log('시작');
let data = fs.readFileSync('./readme.txt');
console.log('1번',data.toString());
data = fs.readFileSync('./readme.txt');
console.log('2번',data.toString());
data = fs.readFileSync('./readme.txt');
console.log('3번',data.toString());
console.log('끝');

// (async () =>{
//     let data = await readFilePromise('./readme.txt');
//     console.log('1',data.toString());
//     data = await readFilePromise('./readme.txt');
//     console.log('2',data.toString());
//     data = await readFilePromise('./readme.txt');
//     console.log('3',data.toString());
// });
// fs.readFile('./readme.txt',(err,data)=>{
//     if(err){
//         throw err;
//     }
//     console.log('1번',data.toString()); //txt read
// });
// fs.readFile('./readme.txt',(err,data)=>{
//     if(err){
//         throw err;
//     }
//     console.log('1번',data.toString()); //txt read
// });
// fs.readFile('./readme.txt',(err,data)=>{
//     if(err){
//         throw err;
//     }
//     console.log('1번',data.toString()); //txt read
// });