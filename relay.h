#ifndef relay
#define relay
#define RELAY_CNT 8

int relays_stats[RELAY_CNT];
boolean switchRelay(int x){
  relays_stats[x] = (relays_stats[x] + 1) % 2;
  digitalWrite(x, relays_stats[x]);
}
boolean switchOn(int x){
  digitalWrite(x, 1);
  relays_stats[x] = 1;
}
boolean switchOff(int x){
  digitalWrite(x,0);
  relays_stats[x] = 0;
}

#endif
