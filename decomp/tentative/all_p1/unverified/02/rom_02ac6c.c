/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02ac6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2ac6c(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) & 0x7f;
  candidate_1b1dc();
  if ((*(byte *)((int)unaff_A6 + 0x91) & 2) == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = 10;
    candidate_4256();
    *unaff_A6 = 0x2ac9e;
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
    if (*(short *)((int)unaff_A6 + 0x82) < 1) {
      *(undefined1 *)(unaff_A6 + 0x18) = 8;
      *unaff_A6 = 0x2acb8;
    }
    candidate_2be44();
    candidate_427a();
    if (*(char *)((int)unaff_A6 + 0x61) == '\x04') {
      *unaff_A6 = candidate_2ad2e;
    }
    if (*(short *)(unaff_A6 + 0xf) < 1) {
      *unaff_A6 = candidate_2b138;
    }
  }
  else {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x82) = uVar1;
    candidate_4256();
    *unaff_A6 = 0x2ad0c;
    candidate_2be44();
    candidate_427a();
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
    if (*(short *)((int)unaff_A6 + 0x82) < 1) {
      *unaff_A6 = candidate_2ad2e;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_2ae46;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2ae46;
  }
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
