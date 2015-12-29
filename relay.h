#ifndef relay
#define relay
#define RELAY_CNT 3

int relays_stats[RELAY_CNT];

boolean switchRelay(int x){
  relays_stats[x-3] = (relays_stats[x-3] + 1) % 2;
  digitalWrite(x, relays_stats[x+3]);
}
boolean switchOn(int x){
  digitalWrite(x, 1);
  relays_stats[x-3] = 1;
}
boolean switchOff(int x){
  digitalWrite(x,0);
  relays_stats[x-3] = 0;
}
int switchState(int x){
  return relays_stats[x-3];
}

#endif
