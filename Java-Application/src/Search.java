
public class Search {
	
	CIDComparator cid;
	CityComparator city;
	FirstNameComparator firstName;
	LastNameComparator lastName;
	AddressLineComparator address;
	SSNComparator ssn;
	Comparatorr a;
	
	public Search(){
		
		cid = new CIDComparator();
		city = new CityComparator();
		firstName = new FirstNameComparator();
		lastName = new LastNameComparator();
		address = new AddressLineComparator();
		ssn = new SSNComparator();
		a = new Comparatorr();
	}
	
	public int BinarySearch(Data [] arr, int low ,int high,int x,int type,String k){ // search in integer rows
		
		switch(type){
	    	case 0 : a = cid; break; 
	      	case 5 : a = ssn; break;
		}
		
		int c = 0;
		if (k.equals("<")) c =-1;
		else if (k.equals(">")) c= 1;

		int middle = 0;
		while(high >= low) {
			middle = low+(high-low)/2;
			if(a.compareForSearch(arr[middle], x) == 0) 
				return middle+c;
			else if(a.compareForSearch(arr[middle], x) < 0) 
				low = middle + 1;
			else if(a.compareForSearch(arr[middle], x) > 0) 
				high = middle - 1;
		}
		if(a.compareForSearch(arr[middle], x) < 0) {
			if (k.equals("<")) return middle;
			else if (k.equals("=")) return -1;
			else return middle+1;
		}
		else {
			if (k.equals("<")) return middle-1;
			else if (k.equals("=")) return -1;
			else return middle;
		}	
		
	}
	public int BinarySearch(Data [] arr, int low ,int high,String x,int type){	// in string type rows
		
		switch(type){
	    	case 1 : a = firstName; break; 
	      	case 2 : a = lastName; break;
	      	case 3 : a = city; break;
	      	case 4 : a = address; break;
		}
		int middle = 0;
		while(high >= low) {
			middle = low+(high-low)/2;
			if(a.compareForSearch(arr[middle], x) == 0) 
				return middle;
			if(a.compareForSearch(arr[middle], x) < 0) 
				low = middle + 1;
			if(a.compareForSearch(arr[middle], x) > 0) 
				high = middle - 1;			
		}
		return middle;	
	}
	
	public Data[] SequantalSearch(Data[] arr,String x,int type){ // after first condition for string conditions
		
		Data [] newList = new Data[arr.length];
		int numb = 0;
		
		switch(type){
    	case 1 : a = firstName; break; 
      	case 2 : a = lastName; break;
      	case 3 : a = city; break;
      	case 4 : a = address; break;
		}
		for(int i = 0 ; i<arr.length; i++){
			if (arr[i] == null) break;
			if (a.isStart(arr[i],x)) 
				newList[numb++] = arr[i];		
		}
		return newList;
	}
	public Data[] SequantalSearch(Data[] arr,int x,int index,int type){
		
		Data [] newList = new Data[arr.length];
		int numb = 0,i;
		switch(type){
	    	case 0 : a = cid; break; 
	      	case 5 : a = ssn; break;
		}
		
		if(a.compareForSearch(arr[index],x) == 0){
			newList[0] = arr[index];
			numb = 1;
		}
		
		for(i=index+1 ; i<arr.length ; i++){ //  after index object equal objects adding
			if (arr[i] == null) break;
			if (a.compareForSearch(arr[i], x) == 0) 
				newList[numb++] = arr[i];
			else break;
		}
		
		for(i = index-1 ; i>0 ; i--){ // before index object equal objects adding 
			if (arr[i] == null) break;
			if (a.compareForSearch(arr[i], x) == 0) 
				newList[numb++] = arr[i];
			else break;
		}
		return newList;
	}
}
