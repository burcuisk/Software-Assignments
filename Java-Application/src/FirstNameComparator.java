
public class FirstNameComparator extends Comparatorr{
	
	public int compare(Data o1, Data o2) {
		if (o1.getFirstName().compareToIgnoreCase(o2.getFirstName()) < 0)
			return -1;
		else if(o1.getFirstName().compareToIgnoreCase(o2.getFirstName()) > 0)
			return 1;
		return 0;
	}
	
	public int compareForSearch(Data a,String x){
		if(a.getFirstName().compareToIgnoreCase(x) < 0)
			return -1;
		else if(a.getFirstName().compareToIgnoreCase(x) > 0)
			return 1;
		return 0;
	}
	
	public boolean isStart(Data a,String x) {
		if(a.getFirstName().toUpperCase().startsWith(x.toUpperCase()))
			return true;
		return false;
	}
}
