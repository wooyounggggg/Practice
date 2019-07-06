const title = document.querySelector("#title");
const CLICKED_CLASS="clicked";

function handleClick(){
  title.classList.toggle('clicked');
//   const currentClassList=title.classList;
//   if(currentClassList.contains('clicked'))
//     currentClassList.remove('clicked');
//   else currentClassList.add('clicked');
}
function init(){
  title.addEventListener("click",handleClick);
}
init();

function handleOffline(){
  console.log('offline!');
}
function handleOnline(){
  console.log('online!');
}
window.addEventListener("offline",handleOffline);
window.addEventListener("online",handleOnline);
// document:Object
// const title=document.querySelector('#title');
   
// window.addEventListener("resize", handleResize);
// const title2=document.querySelector('#title');
// console.log(`title1==title2 : ${title==title2}`);
// console.log(title);
// title.innerHTML="Hi! From JS"
// title.style.color='red';
// document.title="After change"
// search title's object info.
// console.dir(title);

