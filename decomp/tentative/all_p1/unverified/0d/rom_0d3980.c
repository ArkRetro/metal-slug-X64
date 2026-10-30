/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3980.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3980(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  *unaff_A6 = 0xd3992;
  candidate_1abc8();
  if (**(int **)((int)unaff_A6 + 0x82) == 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  *(int **)((int)unaff_A6 + 0x82) = *(int **)((int)unaff_A6 + 0x82) + 1;
  candidate_49e8();
  return;
}
