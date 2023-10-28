all:
	gcc s-talk.c list.c keyboard_input.c udp_input.c udp_output.c screen_output.c -o s-talk

clean:
	rm -f s-talk