/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04af72.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4af72(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  if (*(char *)(unaff_A6[3] + 0xa4) == '\x01') {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x200;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  }
  *unaff_A6 = 0x4afce;
  uVar1 = 0;
  candidate_1abc8();
  candidate_44f6();
  candidate_427a();
  candidate_4ddd4();
  if ((bool)uVar1) {
    if (*(char *)((int)unaff_A6 + 0x9d) == '\x03') {
      *unaff_A6 = candidate_4af72;
    }
    if (*(char *)((int)unaff_A6 + 0x9d) == '\x04') {
      *unaff_A6 = candidate_4afaa;
    }
  }
  if (*(char *)(unaff_A6[3] + 0x9d) == '\x7f') {
    *unaff_A6 = candidate_4b01e;
  }
  return;
}
