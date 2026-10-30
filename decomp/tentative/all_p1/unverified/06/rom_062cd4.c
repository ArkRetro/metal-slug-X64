/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x062cd4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_62cd4(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1af9a();
  if (!(bool)in_CF) {
    candidate_631aa();
    if ((bool)in_CF) {
      candidate_631e0();
      if ((bool)in_CF) {
        candidate_63212();
        *unaff_A6 = 0x62cf8;
        in_CF = 0;
      }
    }
  }
  candidate_1abc8();
  candidate_140c0();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    in_CF = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_4192();
    *unaff_A6 = candidate_62d38;
    in_CF = 0;
  }
  candidate_1945c();
  if (!(bool)in_CF) {
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
