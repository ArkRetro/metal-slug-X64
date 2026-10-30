/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5e88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5e88(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_a6f7a();
  candidate_4256();
  *unaff_A6 = 0xa5ebc;
  candidate_1abc8();
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0xa2) & 8) != 0) {
    *unaff_A6 = candidate_a5eda;
  }
  candidate_1abc8();
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
