// document:Object
const title=document.getElementById('title');
const title2=document.querySelector('#title');
console.log(`title1==title2 : ${title==title2}`);
// console.log(title);
title.innerHTML="Hi! From JS"
title.style.color='red';
document.title="After change"
// search title's object info.
// console.dir(title);

