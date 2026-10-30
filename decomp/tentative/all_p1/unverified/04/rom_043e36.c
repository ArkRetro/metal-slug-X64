/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043e36.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_43e36(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  *(undefined1 *)((int)unaff_A6 + 0x81) = 0;
  if (((0xf < *(short *)(unaff_A6 + 0xf)) && (*(short *)(unaff_A6 + 0xf) < 0x131)) &&
     (*(short *)(unaff_A6 + 0x10) < 0x1f1)) {
    bVar3 = false;
    candidate_40bc();
    if (bVar3) {
      *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)((int)unaff_A6 + 0x6e);
      *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)((int)unaff_A6 + 0x72);
      *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x20);
      bVar3 = true;
    }
    if (bVar3) {
      *(undefined1 *)(extraout_A0 + 0x9b) = *(undefined1 *)((int)unaff_A6 + 0x9b);
      *(undefined4 *)(extraout_A0 + 0x86) = *(undefined4 *)((int)unaff_A6 + 0x86);
      candidate_120f8();
      *(undefined1 *)(unaff_A6 + 0x18) = 0;
      *unaff_A6 = 0x43ea8;
      if (*(char *)(unaff_A6 + 0x18) == '\0') goto LAB_00043ed0;
      *(char *)((int)unaff_A6 + 0x9e) = *(char *)((int)unaff_A6 + 0x9e) + '\x01';
    }
  }
  if (*(char *)((int)unaff_A6 + 0x9e) < *(char *)((int)unaff_A6 + 0x9d)) {
    *unaff_A6 = 0x43ae4;
  }
  else {
    *unaff_A6 = candidate_43ee0;
  }
LAB_00043ed0:
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_1945c();
  if (!(bool)uVar4) {
    candidate_4c24();
    if ((bool)uVar4) {
      *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) | 6;
      if (*(char *)((int)unaff_A6 + 0x5a) == '\f') {
        *unaff_A6 = candidate_43f32;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        *unaff_A6 = candidate_43f32;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if (*(char *)(unaff_A6 + 0x27) != '\0') {
      *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
    }
    candidate_446f0();
    return;
  }
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
