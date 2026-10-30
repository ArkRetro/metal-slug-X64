/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049030.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_49030(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    iVar1 = *(int *)(unaff_A6[3] + 0xc);
    if ((ushort)*(byte *)(iVar1 + 0x96) == *(ushort *)(iVar1 + 0x72)) {
      if (*(char *)(unaff_A6[3] + 0x61) == '\0') {
        *(byte *)(iVar1 + 0x9a) = *(byte *)(iVar1 + 0x9a) | 1;
        *(undefined1 *)(iVar1 + 0x5e) = *(undefined1 *)((int)unaff_A6 + 0x5e);
        *unaff_A6 = candidate_48ab6;
      }
      else {
        *unaff_A6 = candidate_48af6;
      }
    }
    else {
      *unaff_A6 = candidate_48af6;
    }
  }
  if ((*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x9a) & '\x01' << (*(byte *)(unaff_A6[3] + 0x61) & 7)
      ) != 0) {
    *unaff_A6 = candidate_48ab6;
  }
  candidate_4920c();
  return;
}
