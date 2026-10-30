/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ca502.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ca502(void)

{
  ushort uVar1;
  short sVar2;
  ushort uVar3;
  short unaff_D7w;
  char *in_A0;

  sVar2 = 0;
  do {
    uVar1 = *in_A0 + sVar2;
    uVar3 = uVar1 * 2;
    if (CARRY2(uVar1,uVar1)) {
      uVar3 = ~uVar3;
    }
    sVar2 = uVar3 + 0x3c5b;
    unaff_D7w = unaff_D7w + -1;
    in_A0 = in_A0 + 1;
  } while (unaff_D7w != 0);
  return;
}
