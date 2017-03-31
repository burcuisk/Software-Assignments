
public class CIDComparator extends Comparatorr {
	
	public int compare(Data a, Data b) {
		if (a.getCID()<b.getCID())
			return -1;
		else if (a.getCID() > b.getCID())
			return 1;
		else 
			return 0;			
	}
	
	public int compareForSearch(Data a,int b){
		if (a.getCID()>b)
			return 1;
		else if(a.getCID() < b)
			return -1;
		return 0;
	}

}
