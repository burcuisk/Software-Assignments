
public class LastNameComparator extends Comparatorr {

	public int compare(Data o1, Data o2) {
		if (o1.getLastName().compareToIgnoreCase(o2.getLastName()) < 0)
			return -1;
		else if(o1.getLastName().compareToIgnoreCase(o2.getLastName()) > 0)
			return 1;
		return 0;
	}
	
	public int compareForSearch(Data a,String x){
		if(isStart(a,x)) return 0;
		else if (a.getLastName().compareToIgnoreCase(x) < 0)
			return -1;
		else if(a.getLastName().compareToIgnoreCase(x) > 0)
			return 1;
		return 0;
	}
	
	public boolean isStart(Data a,String x) {
		if(a.getLastName().toUpperCase().startsWith(x.toUpperCase()))
			return true;
		return false;
	}
}
