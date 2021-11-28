void setup() {
  Serial.begin(9600);
  String a = "150.308181";
  String A = "";
  int i=0;
  while(a[i]!='.'){
    A+=a[i];
    i++;
  }
  Serial.println(A);
}

void loop() {
  
}
