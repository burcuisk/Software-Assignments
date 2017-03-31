
public class AddressLineComparator extends Comparatorr  {

	public int compare(Data o1, Data o2) { 
		if (o1.getAdress().compareToIgnoreCase(o2.getAdress()) < 0)
			return -1;
		else if(o1.getAdress().compareToIgnoreCase(o2.getAdress()) > 0)
			return 1;
		return 0;
	}
	
	public int compareForSearch(Data a,String x){ 
		if(a.getAdress().compareToIgnoreCase(x) < 0) 
			return -1;
		else if(a.getAdress().compareToIgnoreCase(x) > 0) 
			return 1;
		return 0;
	}
	
	public boolean isStart(Data a,String x) {
		if(a.getAdress().toUpperCase().startsWith(x.toUpperCase()))
			return true;
		return false;
	}
}
