/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06090a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_6090a(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  if (5 < _DAT_0010c2f4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  *(undefined1 *)((int)unaff_A6 + 0x7d) = 1;
  candidate_1ab98();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xffff;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xffff;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0;
  if ((char)_DAT_0010c2f4 == '\x05') {
    candidate_60e3c();
  }
  else {
    candidate_60e02();
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x60974;
  if ((char)_DAT_0010c2f4 == '\x05') {
    cVar3 = '\x15';
  }
  else {
    cVar3 = '\x11';
  }
  if (cVar3 == *(char *)((int)unaff_A6 + 0x61)) {
    *unaff_A6 = candidate_6099e;
  }
  return;
}
