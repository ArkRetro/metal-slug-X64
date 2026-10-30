/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x032f2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_32f2e(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_40bc();
  candidate_1acf2();
  *(undefined1 *)(unaff_A6 + 0x20) = 2;
  *unaff_A6 = 0x32f54;
  candidate_8ccd4();
  candidate_427a();
  uVar3 = *(byte *)(unaff_A6 + 0x20) < 3;
  if (*(byte *)(unaff_A6 + 0x20) == 3) {
    candidate_68e94();
  }
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar3) {
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
