/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0620e6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_620e6(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  if ((bool)in_CF) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    *(undefined1 *)(unaff_A6[3] + 0x60) = 1;
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
  }
  return;
}
