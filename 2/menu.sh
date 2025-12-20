#!/bin/bash
successfull_ops=0
failed_ops=0

show_menu(){
	echo "==============================="
	echo "FILE MANAGMENT MENU (VARIANT 9)"
	echo "==============================="
	echo "1 - Description of this program (HELP)"
	echo "2 - Read text file to stdout"
	echo "3 - Add line numbers and write to another file"
	echo "4 - Print input/output filenames"
	echo "5 - Show statistics"
	echo "6 - Exit"
	echo "==============================="
}

update_stats(){
	if [ "$1" -eq 0 ]; then
	successfull_ops=$((successfull_ops + 1))
	echo "Operation complited successfully."
	echo "Return code: SUCCEESS"
	
else
	failed_ops=$((failed_ops + 1))
	echo "Operation failed."
	echo "Return code: ERROR (exit=$1)"
	fi
}

pause() { read -p "Press Enter to continue..." _; }

while true; do
	show_menu
	read -p "Select an option (1-6): " choice

	case "$choice" in
		1)
			./help_program
			update_stats $?
			pause
			;;
		2) 
			read -p "Enter input filename: " infile
			./read_text "$infile"
			update_stats $?
			pause
			;;
		3)
			read -p "Enter input filename: " infile
			read -p "Enter output filename: " outfile
			./number_lines "$infile" "$outfile"
			update_stats $?
			pause
			;;
		4)
			read -p "Enter input filename: " infile
			read -p "Enter output filename: " outfile
			./print_names "$infile" "$outfile"
			update_stats $?
			pause
			;;
		
		5)
			echo "==============================="
			echo " STATISTICS "
			echo "==============================="
			echo "Succwssfull operations: $successfull_ops"
			echo "Failed operations: $failed_ops"
			echo "Total operations: $((successfull_ops + failed_ops))"
			echo "==============================="
			pause
			;;
		6) 
			echo "Exiting program..."
			break
			;;
		*)
			echo "Invalid option. Please select 1-6."
			failed_ops=$((failed_ops + 1))
			pause
			;;
	esac
done

			echo "==============================="
			echo "FINAL STATISTICS:"
			echo "Successfull operations: $successfull_ops"
			echo "Failed operations: $failed_ops"
			echo "Total operations: $((successfull_ops + failed_ops))"
			echo "==============================="
			
