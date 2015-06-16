import java.io.*;
import java.util.*;
 
class Visitor {
    final Integer startTime;
    final Integer endTime;
    Visitor(Integer startTime, Integer endTime) {
        this.startTime = startTime;
        this.endTime = endTime;
    }
}
 
class Visitors {
    private List<Integer> startTimes = new ArrayList<Integer>();
    private List<Integer> endTimes = new ArrayList<Integer>();
     
    Visitors(List<Visitor> visitors) {
        for(Visitor v : visitors) {
            startTimes.add(v.startTime);
            endTimes.add(v.endTime);
        }
        Collections.sort(startTimes);
        Collections.sort(endTimes);         
    }
     
    int max(int time) {
        int num = 0; 
        for(int i = 0; i < startTimes.size(); i++) { 
            if(time > startTimes.get(i)) 
                num++; 
            if(time > endTimes.get(i)) 
                num--; 
        } 
        return num;        
    }
}
 
public class Main {    
    public static void main(String[] args) throws IOException {
        BufferedReader buf = new BufferedReader(
                new InputStreamReader(System.in));
        System.out.println("输入来访时间与离开时间(0~24)：");
        System.out.println("范例：10 15"); 
        System.out.println("输入-1 -1结束");
         
        List<Visitor> visitors = new ArrayList<Visitor>();
         
        String input = null;
        do { 
            System.out.print(">>"); 
            input = buf.readLine();
            String[] times = input.split(" ");
            visitors.add(
                new Visitor(new Integer(times[0]), new Integer(times[1])));
        } while(!input.equals("-1 -1"));
 
        Visitors vs = new Visitors(visitors);
     
        for(int time = 0; time < 25; time++) { 
            System.out.printf("%d 时的最大访客数：%d%n", time, vs.max(time)); 
        } 
    }
}