
public class SSNComparator extends Comparatorr {
	
	public int compare(Data a, Data b) {
		if (a.getSSN()<b.getSSN())
			return -1;
		else if (a.getSSN() > b.getSSN())
			return 1;
		return 0;
	}
	
	public int compareForSearch(Data a,int x){
		
		if(a.getSSN() < x)
			return -1;
		else if (a.getSSN() > x)
			return 1;
		return 0;
	}

}
