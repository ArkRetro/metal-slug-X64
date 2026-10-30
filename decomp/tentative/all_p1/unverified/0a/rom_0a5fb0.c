/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5fb0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5fb0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x1e;
  *unaff_A6 = 0xa5fbc;
  candidate_1abc8();
  candidate_427a();
  sVar3 = *(short *)((int)unaff_A6 + 0x7e);
  *(short *)((int)unaff_A6 + 0x7e) = sVar3 + -1;
  if (sVar3 == 0) {
    *unaff_A6 = candidate_a5fda;
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
