#include "../header/codexion.h"

void	*routine_coder(void *arg)
{
	t_character	*chara;

	chara = (t_character *)arg;
	while (chara->coder->routine > 0 && chara->state->simu_state)
	{
		if (!chara->state->simu_state)
			break ;
		coder_compile(chara);
		if (!chara->state->simu_state)
			break ;
		printf("\nCoder id.%d is debug\n", chara->coder->id);
		check_state_session(chara->coder->debug, chara);
		if (!chara->state->simu_state)
			break ;
		printf("\nCoder id.%d is refractor\n", chara->coder->id);
		check_state_session(chara->coder->refractor, chara);
		chara->coder->routine--;
	}
	chara->state->coders_done++;
	return (NULL);
}

void	coder_compile(t_character *chara)
{
	// long res;

	// res = convert_time_stamp_coder(chara->coder);
	check_simu_and_check_state(chara);
	gettimeofday(&chara->coder->last_time_compile, NULL);
	if (chara->state->simu_state)
	{
		// printf("\ntime: %ld, Coder id.%d is compile\n", res,  chara->coder->id);
		check_state_session(chara->coder->compile, chara);
		dongle_on_table(chara->coder->left);
		dongle_on_table(chara->coder->right);
	}
}
