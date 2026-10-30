/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c7596.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c7596(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined2 *in_A0;
  undefined2 *in_A1;
  undefined2 *puVar3;

  uVar1 = in_A1[0x16];
  sVar2 = 0x843;
  do {
    puVar3 = in_A1;
    *puVar3 = *in_A0;
    sVar2 = sVar2 + -1;
    in_A0 = in_A0 + 1;
    in_A1 = puVar3 + 1;
  } while (sVar2 != -1);
  puVar3[-0x82d] = uVar1;
  return;
}
