/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04b208.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4b208(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_4256();
  *unaff_A6 = 0x4b21e;
  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  *(undefined2 *)(unaff_A6[3] + 0x98) = *(undefined2 *)(unaff_A6 + 0x26);
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    candidate_4192();
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
