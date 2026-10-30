/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x076506.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_76506(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *unaff_A6 = 0x7651c;
  uVar3 = 0;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_76536;
    uVar3 = 0;
  }
  candidate_1945c();
  if ((bool)uVar3) {
    *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
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
