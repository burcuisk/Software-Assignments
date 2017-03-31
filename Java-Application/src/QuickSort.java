
public class QuickSort {
	CIDComparator cid;
	CityComparator city;
	FirstNameComparator firstName;
	LastNameComparator lastName;
	AddressLineComparator address;
	SSNComparator ssn;
	Comparatorr a;
	
	public QuickSort(){
		
		cid = new CIDComparator();
		city = new CityComparator();
		firstName = new FirstNameComparator();
		lastName = new LastNameComparator();
		address = new AddressLineComparator();
		ssn = new SSNComparator();
		a = new Comparatorr();
	}
    
	public void quickSort(Data [] arr,int lowerIndex, int higherIndex,int type) {
		  
		switch(type){
	    	case 0 : a = this.cid; break; 
	      	case 1 : a = this.firstName; break;
	      	case 2 : a = this.lastName; break;
	      	case 3 : a = this.city; break;
	      	case 4 : a = this.address; break;
	      	case 5 : a = this.ssn; break;
	    }
		  
		int i = lowerIndex;
	    int j = higherIndex;

	    Data pivot =arr[lowerIndex+(higherIndex-lowerIndex)/2];
	    
	    while (i <= j) {
	  
	    	while (a.compare(arr[i], pivot) < 0)  
	    		i++;    
	    	while (a.compare(arr[j], pivot) > 0)
	    		j--;
	    
	        if (i <= j) {
	        	Data temp = arr[i];
	        	arr[i++] = arr[j];
	            arr[j--] = temp;
	        }
	    }
	    if (lowerIndex < j)
	    	quickSort(arr,lowerIndex, j ,type);
	    if (i < higherIndex)
	        quickSort(arr, i, higherIndex,type);
	
	}

}

