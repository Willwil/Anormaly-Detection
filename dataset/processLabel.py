
def main():
	outfile = open ( 'label_USCD1.txt', 'w' )
	trainExample = 34;
	frame = 200
	for i in range(1, trainExample+1):
		for j in range(frame):
			outfile.write('0 ')
		outfile.write("\n")
	
	infile = file('UCSD_Label_Ped1')
	file_content = infile.read() 
	for line in file_content.split('\n'):
		if line=='':
			break
		print line
		A = line.split(':')
		seg = 0;
		length = len(A)
		write0 = True
		for wn in range(1, frame+1):
			if seg < length and wn >= int(A[seg]):
				write0 = not write0;
				seg += 1
			if write0:
				outfile.write('0 ') 
			else:
				outfile.write('1 ') 
		outfile.write("\n")
		
	outfile.close() 
	infile.close()


main()
