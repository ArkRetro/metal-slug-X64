/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b6370.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b6370(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1ac38();
  candidate_427a();
  if ((-0x81 < *(short *)(unaff_A6 + 0xf)) && (*(short *)(unaff_A6 + 0xf) < 0x181)) {
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
