package org.diracvideo.Schroedinger;

class Queue {
    private Picture[] pics;
    private int used;
    
    public Queue(int n) {
	pics = new Picture[n];
	used = 0;
    }

    public synchronized void remove(int n) {
	for(int i = 0; i < pics.length; i++) {
	    if(pics[i] != null &&
	       pics[i].num == n) {
		pics[i] = null;
		used--;
	    }
	}
    }

    public synchronized void add(Picture p) {
	int min = ~(1 << 31), loc = 0;
	for(int i = 0; i < pics.length; i++) {
	    if(pics[i] == null) {
		pics[i] = p;
		used++;
		return;
	    } 
	    if(min > pics[i].num) {
		min = pics[i].num;
		loc = i;
	    }
	}
	pics[loc] = p;
    }

    public synchronized Picture get(int n) throws Exception {
	for(int i = 0; i < pics.length; i++) {
	    if(pics[i] != null &&
	       pics[i].num == n) {
		return pics[i];
	    }
	}
	throw new Exception("Reference picture not found");
    }
    
    public boolean full() {
	return (used >= pics.length -1);
    }
    
}