function three(){
  console.log("I love js");
  throw Error("im an error");
}
function two(){
  three();
}
function one(){
  two();
}
function zero(){
  one();
}

zero();