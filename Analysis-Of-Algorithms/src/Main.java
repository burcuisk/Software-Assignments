import java.util.Random;
import java.util.Scanner;

public class Main{

	public static void main(String[] args) {

		long startTime=0,endTime,totalTimeFinding=0,totalTimeMerge=0,totalTimeBinary=0,totalTimebubble=0;
			
		while(true){
				
			System.out.println("Enter n :");
			Scanner scan = new Scanner(System.in);
			int n = scan.nextInt();

	    	int matrix1[][] = createMatrix(n);
			int matrix2[][] = createMatrix(n);
				   
		    startTime = System.currentTimeMillis();
			matrixMultiplication(matrix1,matrix2,n);
			endTime = System.currentTimeMillis();
			double totalTimeMatrix =((endTime - startTime)/1000.0);
			System.out.println("Matrix Multiplication: "+totalTimeMatrix +" s");
		
		    for (int a=0 ; a<60;a++){
					  
		    	int array[] = createArray(n);
				int array2[] = createArray(n);
					    
				startTime = System.nanoTime();
				findingMaximumElement(array,n);
				endTime = System.nanoTime();
				totalTimeFinding += (endTime - startTime) ;			
				    
				startTime =System.nanoTime();
				array = bubbleSort(array,n);
				endTime = System.nanoTime();
				totalTimebubble +=((endTime - startTime)/1000.0);
					    
				startTime = System.nanoTime();
				mergesort(array2,0,n-1,n);				
				endTime = System.nanoTime();
				totalTimeMerge += ((endTime - startTime)/1000.0);
					    
				Random rand = new Random();
				int x = rand.nextInt(n)+1;
				startTime = System.nanoTime();				
				binarySearch(array2,x,n);
				endTime = System.nanoTime();
				totalTimeBinary += ((endTime - startTime));
			}
			   
				System.out.println("Finding maximum element: "+totalTimeFinding/60 +" ns");
				System.out.println("Bubble sort: "+totalTimebubble/60 + " mikro s.");
				System.out.println("MergeSort: "+totalTimeMerge/60 + " mikro s.");
				System.out.println("BinarySearch: "+totalTimeBinary/60 + " ns");
		}
	    
	} // end main function
	public static int[][] matrixMultiplication(int firstMatrix[][],int secondMatrix[][],int n){ 
		
		int sum = 0, c, d, k;      
	    int multiply[][] = new int[n][n];

	    for ( c = 0 ; c < n ; c++ ){
	    	for ( d = 0 ; d < n ; d++ ){   
	    		for ( k = 0 ; k < n ; k++ ){
	    			sum = sum + firstMatrix[c][k]*secondMatrix[k][d];
	    		}
	    		multiply[c][d] = sum;
	            sum = 0;
	        }
	     }	   
	    return multiply;
	    
	} // end matrix multiplication function
	
	public static int findingMaximumElement (int array[],int n){
		
		int maxElement = array[0];
		
		for(int b=0; b<n; b++) {
			if (maxElement < array[b]) 
				maxElement = array[b];	
		}
		return maxElement;
	} // end finding maximum element
	
	public static int[] bubbleSort(int array[],int n) { 

		int c,d, swap;		 
		
		for (c = 0; c < ( n - 1 ); c++) {
			for (d = 0; d < n - c - 1; d++) {
				if (array[d] > array[d+1]){
					swap       = array[d];
		            array[d]   = array[d+1];
		            array[d+1] = swap;
		        }
		    }
		}
		return array;
	} // end bubbleSort
	
	public static void mergesort(int a[],int left, int right,int n) {
		if (left < right) {
			int middle = left + (right - left) / 2;
			mergesort(a,left, middle,n);
		    mergesort(a,middle + 1, right,n);
		    merge(a,left, (middle+1), right,n);
		 }
	}
	public static void merge(int [] a,int left, int middle, int right,int n) {
		int temp[]= new int[n];
		int i, left_end, num_elements, tmp_pos;
        left_end = (middle - 1);
        tmp_pos = left;
        num_elements = (right - left + 1);

        while ((left <= left_end) && (middle <= right)) {
            if (a[left] <= a[middle])
                temp[tmp_pos++] = a[left++];
            else
                temp[tmp_pos++] = a[middle++];
        }
        while (left <= left_end)
            temp[tmp_pos++] = a[left++];
        
        while (middle <= right)
            temp[tmp_pos++] = a[middle++];

        for (i = 0; i < num_elements; i++) {
            a[right] = temp[right];
            right--;
        }
	} // end merge

	public static boolean binarySearch(int[ ] a, int x,int n) {
 
	    int low = 0;
	    int high = n - 1;
		while(high >= low) {
			int middle = (low + high) / 2;
			if(a[middle] == x) 
				return true;
			if(a[middle] < x) 
				low = middle + 1;
			if(a[middle] > x) 
				high = middle - 1;
		}
		return false;
	} // end binary search

	public static int[] createArray(int n) {
		Random rand = new Random();
		int array[] = new int[n];
		for (int c = 0; c < n; c++) 
			array[c] = rand.nextInt(n) + 1;
		
		return array;	
	}
	
	public static int[][] createMatrix(int n) {
		Random rand = new Random();
		int matrix[][] = new int[n][n];
		
	    for ( int c = 0 ; c < n ; c++ ){
	    	for ( int d = 0 ; d < n ; d++ )
	            matrix[c][d] = rand.nextInt(5) + 1;
		}
	    return matrix;
	}

} // end main class
