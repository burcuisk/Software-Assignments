
public class CityComparator extends Comparatorr {

	public int compare(Data o1, Data o2) {
		if (o1.getCity().compareToIgnoreCase(o2.getCity()) < 0)
			return -1;
		else if(o1.getCity().compareToIgnoreCase(o2.getCity()) > 0)
			return 1;
		return 0;
	}
	
	public int compareForSearch(Data a,String x){
		if(a.getCity().compareToIgnoreCase(x) < 0)
			return -1;
		else if(a.getCity().compareToIgnoreCase(x) > 0)
			return 1;
		return 0;
	}
	
	public boolean isStart(Data a,String x) {
		if(a.getCity().toUpperCase().startsWith(x.toUpperCase()))
			return true;
		return false;
	}

}
