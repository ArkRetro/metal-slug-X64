/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01fa52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1fa52(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_20542();
  if (*(char *)(unaff_A6[3] + 0x60) != '\0') {
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
  if ((*(byte *)(unaff_A6 + 0x23) & 8) != 0) {
    if (((*(char *)(unaff_A6 + 0x18) == '\x03') && (*(char *)((int)unaff_A6 + 0x8a) != '\x03')) &&
       ((*(byte *)(unaff_A6 + 0x23) & 0x10) == 0)) {
      candidate_40bc();
    }
    *(undefined1 *)((int)unaff_A6 + 0x8a) = *(undefined1 *)(unaff_A6 + 0x18);
  }
  return;
}
